一.生成工程步骤：
1. 返回到..\sdk_evn，执行start_cmd.cmd
2. 将目录cd到当前project目录
3. 在cmd命令窗口中输入：generate_project -f -b hpm6750evk -t flash_xip_release

注意：
-b 后面的开发板类型可以通过generate_project -l查看
-t 后面的编译类型可以通过generate_project查看


二.程序运行在RAM中
1. 修改CMakeLists.txt的set(run_in_ram 0)为set(run_in_ram 1)
2. 修改make_project.cmd的set run_in_ram=0为set run_in_ram=1
