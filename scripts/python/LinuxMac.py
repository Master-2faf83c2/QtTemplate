import os
import sys
import subprocess

sys.path.insert(0, os.getcwd())

from scripts.python.Utils import *

args = ReadArgparse()

print(f'开始构建项目: {args.CMAKE_BUILD_TYPE}')
print(f'项目版本号: {args.VERSION}')
cmake_cmd = [
    "cmake",
    "-B", args.BUILD_DIR,
    "-S", ".",
    f"-DCMAKE_BUILD_TYPE:STRING={args.CMAKE_BUILD_TYPE}",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE",
    f"-DTARGET_NAME={args.PROJECT_NAME}",
    f"-DAPP_VERSION={args.VERSION}"
]
result = subprocess.run(cmake_cmd, check=True)

make_cmd = ["make", "-j16"]
result = subprocess.run(make_cmd, cwd=args.BUILD_DIR, check=True)

if args.IS_INSTALL.upper() == "INSTALL":
    install_cmd = ["cmake", "--install", args.BUILD_DIR]
    result = subprocess.run(install_cmd, check=True)
    print("CMake 安装完成")

if result.returncode == 0:
    print("CMake 构建完成")
else:
    print(f"CMake 构建失败, 错误码: {result.returncode}")

if args.IS_INSTALL.upper() != "INSTALL":
    src_dir = f"{args.ROOT_DIR}/config"
    dst_dir = f"{args.BUILD_DIR}/config"
    CopyFilesAndFolders(src_dir, dst_dir)
