import os
import sys
import subprocess

sys.path.insert(0, os.getcwd())

from scripts.python.Utils import *

args = ReadArgparse()

program_files_x86 = os.environ.get("ProgramFiles(x86)")
if not program_files_x86:
    raise RuntimeError("找不到 ProgramFiles(x86) 环境变量")

vswhere_path = os.path.join(
    program_files_x86,
    "Microsoft Visual Studio",
    "Installer",
    "vswhere.exe"
)

#检查 vswhere 是否存在
if not os.path.exists(vswhere_path):
    raise FileNotFoundError(f"vswhere.exe 未找到: {vswhere_path}")

#调用 vswhere
result = subprocess.run(
    [vswhere_path, "-property", "installationPath"],
    capture_output=True,
    text=True,
    check=True
)

vs_path = result.stdout.strip()
vs_path = os.path.join(vs_path, "VC", "Auxiliary", "Build", "vcvars64.bat")
print("VS 安装路径:", vs_path)

print(f'开始构建项目: {args.CMAKE_BUILD_TYPE}')
print(f'项目版本号: {args.VERSION}')

cmd_list = []

vs_cmd = [
    vs_path
]

cmake_cmd = [
    "cmake",
    "-G", "Ninja",
    "-B", args.BUILD_DIR,
    "-S", ".",
    f"-DCMAKE_BUILD_TYPE:STRING={args.CMAKE_BUILD_TYPE}",
    "--no-warn-unused-cli",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE",
    f"-DTARGET_NAME={args.PROJECT_NAME}",
    f"-DAPP_VERSION={args.VERSION}"
]

if args.IS_INSTALL.upper() == "INSTALL":
    cmake_cmd.append("-DIS_INATLL:BOOL=TRUE")
    print(f'需要安装')


build_cmd = [
    "cmake",
    "--build",
    args.BUILD_DIR,
    "-j16"
]

cmd_list.append(vs_cmd)
cmd_list.append(cmake_cmd)
cmd_list.append(build_cmd)

if args.IS_INSTALL.upper() == "INSTALL":
    install_cmd = [
        "cmake",
        "--install",
        args.BUILD_DIR,
        "--config",
        args.CMAKE_BUILD_TYPE
    ]
    cmd_list.append(install_cmd)
else: 
    src_dir = f"{args.ROOT_DIR}/config"
    dst_dir = f"{args.BUILD_DIR}/config"
    CopyFilesAndFolders(src_dir, dst_dir)

run_cmd = []

for idx, c in enumerate(cmd_list):
    for c_str in c:
        run_cmd.append(c_str)
    if idx != len(cmd_list) - 1:
        run_cmd.append("&&")

print(f'run_cmd: {run_cmd}')

result = subprocess.run(run_cmd, cwd=args.ROOT_DIR)
if not result.returncode == 0:
    print(f"CMake 构建编译失败, 错误码: {result.returncode}")
    exit()

print("CMake 构建编译成功!")


