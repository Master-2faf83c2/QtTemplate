import os
import shutil
import argparse

def ReadArgparse():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "ROOT_DIR",
        type=str,
        default=os.getcwd(),
        help="工作区目录"
    )
    parser.add_argument(
        "PROJECT_NAME",
        type=str,
        default="app",
        help="项目名称"
    )
    parser.add_argument(
        "BUILD_DIR",
        type=str,
        default="build",
        help="输出目录"
    )
    parser.add_argument(
        "CMAKE_BUILD_TYPE",
        type=str,
        default="Debug",
        help="构建版本"
    )
    parser.add_argument(
        "IS_INSTALL",
        type=str,
        default="NO_INSTALL",
        help="是否安装"
    )
    parser.add_argument(
        "VERSION",
        type=str,
        default="v0.0.1",
        help="程序版本号"
    )

    return parser.parse_args()

def CopyFiles(src_dir, dst_dir):
    os.makedirs(dst_dir, exist_ok=True)
    
    for file_name in os.listdir(src_dir):
        src_file = os.path.join(src_dir, file_name)
        dst_file = os.path.join(dst_dir, file_name)
        
        #仅复制文件(跳过子文件夹)
        if os.path.isfile(src_file):
            shutil.copy2(src_file, dst_file)
            print(f"已复制：{src_file} → {dst_file}")

def CopyFilesAndFolders(src_dir, dst_dir):
    if not os.path.exists(src_dir):
        print(f"源目录不存在：{src_dir}")
        return

    os.makedirs(dst_dir, exist_ok=True)

    for name in os.listdir(src_dir):
        src_path = os.path.join(src_dir, name)
        dst_path = os.path.join(dst_dir, name)

        if os.path.isfile(src_path):
            #复制文件
            shutil.copy2(src_path, dst_path)
            # print(f"已复制文件：{src_path} → {dst_path}")
        elif os.path.isdir(src_path):
            #递归复制子目录
            CopyFilesAndFolders(src_path, dst_path)
