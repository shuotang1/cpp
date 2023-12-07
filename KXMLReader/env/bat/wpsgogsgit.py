import argparse 
import requests
import json
import re
import os
import subprocess
import sys
import csv
import shutil
from xpinyin import Pinyin

parser = argparse.ArgumentParser(description = 'wpsgogsgit')
parser.add_argument('menu', choices=['checkOrgExist', 'createOrg', 'pullCode', 'createUser', 'createRepo', 'bindUserAndRepo', 'delete', 'compile'], help='菜单项')
parser.add_argument('root_url', help='root url')
parser.add_argument('access_token', help='access token')
#可选参数
parser.add_argument('--org_name', default='', help='org name')
parser.add_argument('--org_description', default='', help='org description')
parser.add_argument('--pullCodeOutPath', default='', help='pullCodeOutPath')
parser.add_argument('--current_path', default='', help='wpsgogsgit.exe run dir path')
parser.add_argument('--repo_private', default='', help='repo is or not is private')
parser.add_argument('--isSetdefaultfile',default='',help='Whether to set the repo default file')
parser.add_argument('--repo_defaultFile', default='', help='repo default file path')
parser.add_argument('--deleteMenu',default='',help='delete Menu')
parser.add_argument('--compile_path',default='',help='compile path')

args = parser.parse_args()
menu = args.menu
root_url = args.root_url
api_url = root_url + 'api/v1'
access_token = args.access_token
org_name = args.org_name
org_description = args.org_description
pullCodeOutPath = args.pullCodeOutPath
repo_private = args.repo_private
repo_isSetdefaultfile = args.isSetdefaultfile
default_filepath = args.repo_defaultFile
deleteMenu = args.deleteMenu
compile_path = args.compile_path

current_path = args.current_path
csv_file = current_path + r"\venv\userList.csv"
res_csv_file = current_path + r"\venv\resCsvFile.csv"
repoList_csv_file = current_path + r"\venv\repoList.csv"
delete_csv_file = current_path + r"\venv\delete.csv"
xpinyin_path = current_path + r"\venv\xpinyin\Mandarin.dat"

PROJECT_TYPE = ".sln"
OUTPUT_DIRECTORY = "exec"
log_file = 'build.log'
msbuildPath = 'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe'

local_path = current_path + r'\venv\temp'

def create_org(org_name, org_description, root_url, api_url, access_token):
    url = f"{api_url}/user/orgs"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    data = {
        "username": org_name,
        "description": org_description,
    }
    # 发送 API 请求
    print(json.dumps(data))
    try:
        response = requests.post(url, headers=headers, data=json.dumps(data))
    except requests.exceptions.RequestException as e:
        print(f"请求发生异常：{e}")
    print(response.status_code)
    if response.status_code != 201:
        print(response.json())
    else:
        print(f"{root_url}{org_name}")


def check_org_exist(org_name, api_url, access_token):
    url = f"{api_url}/orgs/{org_name}"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    try:
        response = requests.get(url, headers=headers)
    except requests.exceptions.RequestException as e:
        print(f"发生异常：{e}")
    if response.status_code == 200:
        print("exist")
        return True
    else:
        print("notExist")
        return False

def get_org_repository_list(org_name, api_url, access_token):
    url = f"{api_url}/orgs/{org_name}/repos"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    repository_list = []
    try:
        response = requests.get(url, headers=headers)
    except requests.exceptions.RequestException as e:
        print(f"请求发生异常：{e}")
    if response.status_code == 200:
        repositories = response.json()
        for repo in repositories:
            repo_url = repo["html_url"]
            repository_list.append(repo_url)
    else:
        print("获取仓库列表失败")
    return repository_list

def pullCode(repository_list, outputDir):              #git仓库拉取远方代码
    for repo in repository_list:
        name = repo.split("/")[-1]                    #获取仓库名称
        cwd = os.path.join(outputDir, name)       #将baseDir和name两个路径合并为一个新的路径
        if os.path.exists(cwd) == False:
            os.chdir(outputDir)                   #将当前工作目录更改为baseDir目录
            print('clone ' + repo)
            os.system('git clone ' + repo)  #克隆git仓库的url
        else:
            os.chdir(cwd)
            print('pull ' + repo)
            os.system('git pull')               #从远程仓库拉取最新提交并合并到当前分支

def ChineseToPinyin(namesList): 
    p = Pinyin(xpinyin_path)  
    for index, name in enumerate(namesList):
        result1 = p.get_pinyin(name)
        s = result1.split('-')
        result3 = s[0].capitalize() + ''.join(s[1:]).capitalize()
        namesList[index] = result3.lower()

def get_account_data(csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck):
    with open(csv_file, "r") as file:
        reader = csv.reader(file)
        next(reader)
        for row in reader:
            if row[0]=='':
                continue;
            columnName.append(row[0])
            columnRepo.append(row[0])
            columnPassword.append(row[1])
            columnEmail.append(row[2])
            columnCheck.append(row[2])
    ChineseToPinyin(columnRepo)

def create_gogs_account(namelist, passwordlist, emaillist, columnCheck, api_url, access_token):
    for index, (username, password, email) in enumerate(zip(namelist, passwordlist, emaillist)):
        newusername = get_available_username(username, api_url, access_token)
        namelist[index]=newusername
        data = {
            "username": newusername,
            "password": password,
            "email": email
        }
        headers = {
            "Authorization": f"token {access_token}",
            "Content-Type": "application/json"
        }
        try:
            response = requests.post(api_url + "/admin/users", json=data, headers=headers)
            if response.status_code == 201:
                columnCheck[index] = 'True'
                print(f"用户 {newusername} 添加成功！")
            else:
                columnCheck[index] = 'False'
                print(f"用户 {newusername} 添加失败：{response.text}")
        except requests.exceptions.RequestException as e:
            columnCheck[index] = 'False'
            print(f"添加用户 {newusername} 时发生异常：{e}")
            continue

def get_available_username(username, api_url, access_token):
    suffix = 1
    new_username = username
    while not check_username_availability(new_username, api_url, access_token):
        suffix += 1
        new_username = f"{username}{suffix}"
    return new_username

def check_username_availability(username, api_url, access_token):
    #query_url = f"{root_url}api/v1/users/{username}"
    query_url = f"{api_url}/users/{username}"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    try:
        response = requests.get(query_url, headers=headers)
    except requests.exceptions.RequestException as e:
        print(f"发生异常：{e}")
    if response.status_code == 200:
        return False
    elif response.status_code == 404:
        return True
    else:
        raise Exception(f"查询用户名时发生错误：{response.text}")

def write_res_csv(res_csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck):
    with open(res_csv_file, mode="w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["姓名", "密码", "邮箱", "账号名", "成功与否"])  # 写入表头
        for i in range(len(columnName)):
            writer.writerow([columnName[i], columnPassword[i], columnEmail[i], columnRepo[i], columnCheck[i]])

def write_repoList_csv(repoList_csv_file, columnRepo):
    with open(repoList_csv_file, mode = "w", newline = "") as file:
        writer = csv.writer(file)
        writer.writerow(["仓库名", "关联账号名"])
        for i in range(len(columnRepo)):
            writer.writerow([columnRepo[i],columnRepo[i]])

def set_account_data():
    data = zip(columnName, columnPassword)
    with open(output_csv_file, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["账号", "密码"])
        writer.writerows(data)
        #这里的账户是非法的
    print("Column 1:", columnName)
    print("Column 2:", columnPassword)

def create_gogs_group(org_name, repo_private, file_path, root_url, api_url, access_token):
    userlist = []
    with open(file_path, "r") as f:
        reader = csv.reader(f)
        next(reader)  # 跳过表头
        for row in reader:
            userlist.append(row[0])

    for repo_name in userlist:
        url = f"{api_url}/org/{org_name}/repos"
        headers = {
            "Authorization": f"token {access_token}",
            "Content-Type": "application/json"
        }
        data = {
            "name": repo_name,
            "private": repo_private
        }
        response = requests.post(url, headers=headers, json=data)
        if response.status_code == 201:
            print(f"成功创建仓库：{repo_name}")
            print(f"{root_url}{org_name}/{repo_name}")
        else:
            print(f"创建仓库失败：{repo_name}，错误信息：{response.json().get('message')}")

    return userlist


def set_default_groupfile(default_filepath, repo_url, local_path, repo_name):
    os.chdir(local_path)
    subprocess.call(["git", "clone", repo_url])
    os.chdir(local_path + '/' + repo_name)
    readme_paths = copy_folder_with_readme(default_filepath, local_path + '/' + repo_name)
    for readme_path in readme_paths:
        subprocess.call(["git", "add", readme_path])
    subprocess.call(["git", "commit", "-m", "Add default files"])
    subprocess.call(["git", "push"])


def group_set_default_groupfile(local_path, default_filepath, userlist , org_name, root_url):
    for repo_name in userlist:
        repo_url = root_url + org_name + '/' + repo_name + '.git'
        set_default_groupfile(default_filepath, repo_url, local_path, repo_name)


def copy_folder_with_readme(source_folder, destination_folder):
    shutil.copytree(source_folder, destination_folder, dirs_exist_ok=True)
    readme_paths = []
    for root, dirs, files in os.walk(destination_folder):
        for file in files:
            if file.lower() == "readme.md":
                readme_path = os.path.join(root, file)
                readme_paths.append(readme_path)

    return readme_paths

def add_member_to_repo(username, repo_name, member_name, api_url, access_token, root_url):
    endpoint = f"{api_url}/repos/{username}/{repo_name}/collaborators/{member_name}"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    data = {
        "permission": "write"  # 成员的权限（read, write, admin）
    }

    try:
        response = requests.put(endpoint, headers=headers, data=json.dumps(data))
    except requests.exceptions.RequestException as e:
        print(f"发生异常：{e}")
    print(response.status_code)
    if response.status_code == 201 or response.status_code == 204:
        print(f"成功将 {member_name} 添加为仓库 {username}/{repo_name} 的成员。")
        print(f"{root_url}{username}/{repo_name}/settings/collaboration")
    else:
        print(f"添加成员失败:{response.text}")


def associate_user_and_repository(org_name, file_path, api_url, access_token, root_url):
    userlist = []
    repolist = []
    with open(file_path, "r") as f:
        reader = csv.reader(f)
        next(reader)  # 跳过表头
        for row in reader:
            userlist.append(row[1])
            repolist.append(row[0])

    for index,user in enumerate(userlist):
        add_member_to_repo(org_name, repolist[index], userlist[index], api_url, access_token, root_url)

def list_org_repo(cpp_org_name, api_url, access_token):
    url = f"{api_url}/orgs/{cpp_org_name}/repos"
    headers = {
        "Authorization": f"token {access_token}",
        "Content-Type": "application/json"
    }
    repos = []
    try:
        response = requests.get(url, headers=headers)
    except requests.exceptions.RequestException as e:
        print(f"发生异常：{e}")
    repositories = response.json()
    if response.status_code == 200:
        for repository in repositories:
            repos.append(repository["name"])
    else:
        print("Failed to fetch repositories.")
    return repos


def list_org_Account(cpp_org_name, repoList, api_url, access_token):
    userNameList = []
    loginList = []
    emailList = []
    adminList = []
    for repo in repoList:
        url = f"{api_url}/repos/{cpp_org_name}/{repo}/collaborators"
        headers = {
            "Authorization": f"token {access_token}",
            "Content-Type": "application/json"
        }
        try:
            response = requests.get(url, headers=headers)
            response.raise_for_status()  # 检查请求是否成功
            repositories = response.json()
            if response.status_code == 200:
                for repository in repositories:
                    userNameList.append(repository["username"])
                    loginList.append(repository["login"])
                    emailList.append(repository["email"])
                    adminList.append(repository["permissions"]["admin"])
            else:
                print("Failed to fetch repositories.")
        except requests.exceptions.RequestException as e:
            print(f"发生异常：{e}")
        except json.JSONDecodeError as e:
            print(f"JSON解码错误：{e}")
    return userNameList, loginList, emailList, adminList

def delete_org_account(userNameList, adminList, api_url, access_token):
    for index, userName in enumerate(userNameList):
        if adminList[index] == True:
            print(f"由于{userName}是管理员,因此并未删除")
            continue;
        url = f"{api_url}/admin/users/{userName}"
        headers = {
            "Authorization": f"token {access_token}",
            "Content-Type": "application/json"
        }
        try:
            response = requests.delete(url, headers=headers)
        except requests.exceptions.RequestException as e:
            print(f"发生异常：{e}")
        if response.status_code == 204:
            print(f"{userName}删除成功")
        else:
            print(f"{userName}删除失败")

def delete_org_repo(cpp_org_name, repoList, api_url, access_token, root_url):
    for repo in repoList:
        url = f"{api_url}/repos/{cpp_org_name}/{repo}"
        headers = {
            "Authorization": f"token {access_token}",
            "Content-Type": "application/json"
        }
        try:
            response = requests.delete(url, headers=headers)
        except requests.exceptions.RequestException as e:
            print(f"发生异常：{e}")
        if response.status_code == 204:
            print(f"{repo}删除成功")
        else:
            print(f"{repo}删除失败")
    print(f"{root_url}{cpp_org_name}")

def write_deleteList_csv(filePath, repoList, userNameList, loginList, emailList):
    with open(filePath, mode="w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["仓库名", "用户名", "登录账号", "邮箱"])
        for i in range(len(repoList)):
            try:
                writer.writerow([repoList[i], userNameList[i], loginList[i], emailList[i]])
            except IndexError:
                writer.writerow([repoList[i], "", "", ""])


def delete_files_and_dirs(root_path):
    for root, dirs, files in os.walk(root_path, topdown=False):
        for file in files:
            file_path = os.path.join(root, file)
            if file_path.endswith('.tlog') \
                or file_path.endswith('.lastbuildstate') \
                or file_path.endswith('.idb') \
                or file_path.endswith('.exe') \
                or file_path.endswith('.pdb') \
                or file_path.endswith('.ilk') \
                or file_path.endswith('.obj'):
                os.remove(file_path)    #删除指定路径的文件
                print(f"Deleted {file_path}")
        for dir in dirs:
            dir_path = os.path.join(root, dir)
            if not os.listdir(dir_path):  #检查指定目录是否为空
                os.rmdir(dir_path)

def build_solution(root_dir,working_dir,sln_file_path,f):
    msbuild_cmd = [msbuildPath, '/m', sln_file_path, '/p:Configuration=Debug',
                   '/flp1:errorsonly;logfile={0}'.format(log_file)];
    try:
        subprocess.check_call(msbuild_cmd,cwd=working_dir)  # 在指定工作路径下执行musbuild_cmd命令
        #with open(log_file,'a') as f1:
        #   subprocess.check_call(msbuild_cmd, cwd=working_dir,stdout=f1,stderr=f1)
        return True
    except subprocess.CalledProcessError as error:
        f.write(sln_file_path)
        f.write('\n')
        return False

def copy_output_files(project_path):
    #print("output_path: "{os.path.dirname(project_path)})
    output_path = os.path.join(os.path.dirname(project_path), OUTPUT_DIRECTORY)
    #print(output_path)
    os.makedirs(output_path, exist_ok=True)      #创建指定目录
    for root, dirs, files in os.walk(project_path):
        for file in files:
            if file.endswith('.exe'):
                src_path = os.path.join(root, file)
                dst_path = os.path.join(output_path, file)
                shutil.copy(src_path, dst_path)   #将源文件或目录从src_path复制到目标位置dst_path

def compile_sln_files(root_dir):       #编译指定目录下的sln文件
    errerlist_file = root_dir + '\\errorlist.txt'
    f = open(errerlist_file, 'w')
    for root, dirs, files in os.walk(root_dir):                 #os.walk()函数遍历指定目录下所有文件和目录
        for name in files:
            if re.search(r'\.sln$', name):
                sln_file_path=os.path.join(root,name)
                working_dir=os.path.dirname(sln_file_path)
                #print(working_dir)
                if(build_solution(root_dir,working_dir,sln_file_path,f)):
                    copy_output_files(working_dir)
                    delete_files_and_dirs(working_dir)

if menu == 'checkOrgExist':
    check_org_exist(org_name, api_url, access_token)  # --TODO bug:当org_name=1时，即使原仓库不存在org_name，也会显示存在
elif menu == 'createOrg':
    create_org(org_name, org_description, root_url, api_url, access_token)
elif menu == 'pullCode':
    repository_list = get_org_repository_list(org_name, api_url, access_token)
    pullCode(repository_list, pullCodeOutPath)
elif menu == 'createUser':
    columnName = []
    columnPassword = []
    columnEmail = []
    columnRepo = []
    columnCheck = []
    get_account_data(csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck)
    create_gogs_account(columnRepo, columnPassword, columnEmail, columnCheck, api_url, access_token)
    write_res_csv(res_csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck)
    write_repoList_csv(repoList_csv_file, columnRepo)
elif menu == 'createRepo':
    if repo_private == '1':
        repo_private = True
    else:
        repo_private = False
    if default_filepath == '':
        default_filepath = current_path + r"\default"
    else:
        default_filepath = sys.argv[4]
    cpplist = create_gogs_group(org_name, repo_private, repoList_csv_file, root_url, api_url, access_token)
    if repo_isSetdefaultfile == '1':
        group_set_default_groupfile(local_path, default_filepath, cpplist, org_name, root_url)
elif menu == 'bindUserAndRepo':
    associate_user_and_repository(org_name, repoList_csv_file, api_url, access_token, root_url)
elif menu == 'delete':
    repoList = list_org_repo(org_name, api_url, access_token)
    userNameList, loginList, emailList, adminList = list_org_Account(org_name, repoList, api_url,
                                                                                   access_token)
    if deleteMenu == 'listRepoUser':
        write_deleteList_csv(delete_csv_file, repoList, userNameList, loginList, emailList)
    elif deleteMenu == 'deleteUser':
        delete_org_account(userNameList, adminList, api_url, access_token)
    elif deleteMenu == 'deleteRepo':
        delete_org_repo(org_name, repoList, api_url, access_token, root_url)
elif menu == 'compile':
    compile_sln_files(compile_path)


