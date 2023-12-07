@echo off

rem 获取BAT文件所在目录路径
set "bat_dir=%~dp0"

rem 获取上一级目录的路径
set "parent_dir=%bat_dir%\..\"

rem 设置编译输出目录为 debug 目录  
set "output_dir=%parent_dir%\debug" 

rem 创建 debug 目录（如果不存在）  
if not exist "%output_dir%" (  
    mkdir "%output_dir%"  
)  

rem 导航到编译输出目录  
pushd "%output_dir%" 

rem 设置 Qt 所在的路径为参数  
set "qt_path=D:/qt/qt/5.9.3/msvc2017_64"

rem 运行 cmake 命令进行配置，传递 Qt 路径参数  
cmake -DQT_PATH="%qt_path%" "%parent_dir%\coding"  

setlocal enabledelayedexpansion  
  
rem 获取脚本所在目录的上一级目录路径  
for %%A in ("%~dp0..") do set "parent_dir=%%~fA"  
  
rem 创建目标文件夹（如果不存在）  
set "target_dir=%parent_dir%\debug\Debug\env"  
mkdir "%target_dir%" 2>nul 

pushd "%bat_dir%"
  
rem 读取文件中的每一行  
for /f "tokens=*" %%a in (download_list.txt) do (  
  rem 忽略以 REM 开头的行（注释行）  
  echo %%a | findstr /i "^REM" > nul  
  if errorlevel 1 (  
    rem 解析远程地址和本地文件名  
    for /f "tokens=1,2 delims=," %%b in ("%%a") do (  
	
      rem 构建本地文件路径  
      set "local_file=%target_dir%\%%c"  
        
      rem 下载远程文件到本地  
      echo 正在下载 %%b 至 !local_file!  
      curl -o "!local_file!" -u kmeeting:Wps@teacher2023 "file://%%b"  
        
      if not errorlevel 1 (  
        echo 下载成功！  
      ) else (  
        echo 下载失败！  
      )  
    )  
  )  
)  
  
endlocal  

pushd "%bat_dir%"

rem 切换到bat脚本同级目录下的7z文件夹  
cd /d "%~dp0\7z"  
  
rem 解压Debug文件夹下env文件夹中所有的7z压缩包  
for %%F in ("%~dp0\..\debug\Debug\env\*.7z") do (  
    7z x "%%F" -o"%~dp0\..\debug\Debug\env"  
)  
  
echo 解压完成！

rem 运行 make 命令进行编译（假设使用 make）  
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe" cpp.sln /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v142 /p:CXXLanguageStandard=std17  

rem 返回到原始目录  
popd  