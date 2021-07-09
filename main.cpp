#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <wingdi.h>

const DWORD MAX_WIDTH = 2736;
const DWORD MAX_HEIGHT = 1824;
const UINT FULL_SCALING = -4; // 100%
const UINT NEARLY_DOUBLE_SCALING = -1; // 175%

using namespace std;

void printDisplayInformations(DEVMODEA devMode) {
    printf("\t\tWidth: %d\n", devMode.dmPelsWidth);
    printf("\t\tHeight: %d\n", devMode.dmPelsHeight);
    printf("\t\tFrequency: %d\n", devMode.dmDisplayFrequency);
    printf("\t\tBitsPer: %d\n", devMode.dmBitsPerPel);
    printf("\t\tFlags: %d\n", devMode.dmDisplayFlags);
    printf("\n");
}

int changeResScaling() {
    bool prevMaxRes = false;
    DEVMODEA devMode;
    devMode.dmSize = sizeof(DEVMODE);

    if (!EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        cerr << "ディスプレイの情報の取得に失敗しました。" << endl;
        return 1;
    }

    printf("現在の設定:\n");
    printDisplayInformations(devMode);

    devMode.dmFields = DM_BITSPERPEL |
        DM_PELSWIDTH |
        DM_PELSHEIGHT |
        DM_DISPLAYFLAGS |
        DM_DISPLAYFREQUENCY;

    if (devMode.dmPelsWidth != MAX_WIDTH ||
        devMode.dmPelsHeight != MAX_HEIGHT) {
        cout << "解像度をもとに戻そうとしています. . ." << endl << endl;

        devMode.dmPelsWidth = MAX_WIDTH;
        devMode.dmPelsHeight = MAX_HEIGHT;
        prevMaxRes = true;
    }
    else {
        devMode.dmPelsWidth = 1920;
        devMode.dmPelsHeight = 1200;
    }

    printf("変更予定:\n");
    printDisplayInformations(devMode);

    LONG changeDisplay = ChangeDisplaySettingsA(&devMode, 0);
    if (changeDisplay != DISP_CHANGE_SUCCESSFUL) {
        cerr << "解像度の変更に失敗しました。" << endl
            << "コード: " << changeDisplay << endl;
        return 1;
    }

    cout << "解像度を変更しました。" << endl;
    cout << "ディスプレイスケーリングを変更しようとしています. . ." << endl;

    UINT scale = prevMaxRes ? NEARLY_DOUBLE_SCALING : FULL_SCALING;
    if (!SystemParametersInfo(SPI_SETLOGICALDPIOVERRIDE, scale, NULL, 1)) {
        cerr << "ディスプレイスケーリングを変更できませんでした。" << endl;
        return 1;
    }

    cout << "ディスプレイスケールを変更しました。" << endl << endl;
    return 0;
}

int main() {
    int exit_code = changeResScaling();
    system("pause");

    return exit_code;
}