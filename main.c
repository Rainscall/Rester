#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void restartProcess(const char *filePath, const char *processName, int seconds) {
    // 启动进程
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    char commandLine[256];
    sprintf(commandLine, "\"%s\" %s", filePath, processName);

    if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Process started successfully.\n");

        for (int remainingSeconds = seconds; remainingSeconds > 0; remainingSeconds--) {
            printf("Remaining time: %d seconds\n", remainingSeconds);
            Sleep(1000); // 每秒休眠一次
        }

        printf("Restarting the process...\n");
        printf("Notice: To exit, use Ctrl+C\n");

        // 结束进程
        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        restartProcess(filePath, processName, seconds); // 递归重新启动进程
    } else {
        printf("Failed to start the process. Error %d\n", GetLastError());
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        // 如果没有提供三个参数，则询问用户输入
        char filePath[256], processName[256];
        int seconds;

        printf("Enter executable file path: ");
        fgets(filePath, sizeof(filePath), stdin);
        filePath[strcspn(filePath, "\n")] = '\0';  // 移除换行符

        printf("Enter process name: ");
        fgets(processName, sizeof(processName), stdin);
        processName[strcspn(processName, "\n")] = '\0';  // 移除换行符

        printf("Enter time in seconds: ");
        scanf("%d", &seconds);

        restartProcess(filePath, processName, seconds);
    } else {
        const char *filePath = argv[1];
        const char *processName = argv[2];
        const char *timeString = argv[3];

        if (filePath[0] == '\0' || processName[0] == '\0' || timeString[0] == '\0') {
            printf("Please provide all three arguments.\n");
            return 1;
        }

        int seconds = atoi(timeString);

        if (seconds <= 0) {
            printf("Invalid time specified.\n");
            return 1;
        }

        restartProcess(filePath, processName, seconds);
    }

    return 0;
}
