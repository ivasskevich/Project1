#include <Windows.h>
#include "resource.h"
#include <vector>
#include <ctime>

HINSTANCE hInstance;
bool isPlayerTurn = true;
bool isCirclePlaced = false;
bool isRadio2Selected = false;
std::vector<int> moves;
HWND lastSelectedRadioButton = nullptr;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    ::hInstance = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

void MakeMove(HWND hwnd, int controlId, HBITMAP hBitmap)
{
    if (std::find(moves.begin(), moves.end(), controlId) != moves.end())
        return;

    HWND hButton = GetDlgItem(hwnd, controlId);
    SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
    moves.push_back(controlId);
}

bool IsMoveAvailable(int controlId)
{
    return (std::find(moves.begin(), moves.end(), controlId) == moves.end());
}

void ComputerMove(HWND hwnd)
{
    std::vector<int> availableMoves;
    for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
        if (IsMoveAvailable(i))
            availableMoves.push_back(i);
    }

    if (!availableMoves.empty()) {
        srand((unsigned int)time(NULL));
        int randomIndex = rand() % availableMoves.size();
        int randomMove = availableMoves[randomIndex];
        HBITMAP hNewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
        MakeMove(hwnd, randomMove, hNewBitmap);
    }
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        UINT buttonIds[] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3,
                             IDC_BUTTON4, IDC_BUTTON5, IDC_BUTTON6,
                             IDC_BUTTON7, IDC_BUTTON8, IDC_BUTTON9 };

        HBITMAP hEmptyBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
        for (int i = 0; i < sizeof(buttonIds) / sizeof(buttonIds[0]); ++i) {
            HWND hButton = GetDlgItem(hwnd, buttonIds[i]);
            SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hEmptyBitmap);
        }

        if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
            isPlayerTurn = true;
            isRadio2Selected = true;
            if (!isCirclePlaced) {
                isCirclePlaced = true;
                HBITMAP hNewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
                MakeMove(hwnd, IDC_BUTTON1, hNewBitmap);
            }
        }
        else {
            isPlayerTurn = false;
            isRadio2Selected = false;
        }

        return TRUE;
    }

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;

    case WM_COMMAND:
    {
        int controlId = LOWORD(wParam);
        int notification = HIWORD(wParam);

        if (notification == BN_CLICKED) {
            if (controlId >= IDC_BUTTON1 && controlId <= IDC_BUTTON9) {
                if (isPlayerTurn && IsMoveAvailable(controlId)) {
                    HBITMAP hNewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
                    MakeMove(hwnd, controlId, hNewBitmap);
                    ComputerMove(hwnd);
                }
            }
            else if (controlId == IDC_BUTTON10) {
                moves.clear();
                isCirclePlaced = false;
                lastSelectedRadioButton = nullptr;
                HBITMAP hEmptyBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
                for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
                    HWND hButton = GetDlgItem(hwnd, i);
                    SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hEmptyBitmap);
                }

                if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED && !isCirclePlaced) {
                    isCirclePlaced = true;
                    std::vector<int> availableButtons;
                    for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
                        if (IsMoveAvailable(i))
                            availableButtons.push_back(i);
                    }
                    if (!availableButtons.empty()) {
                        srand((unsigned int)time(NULL));
                        int randomIndex = rand() % availableButtons.size();
                        int buttonId = availableButtons[randomIndex];
                        HBITMAP hCircleBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
                        MakeMove(hwnd, buttonId, hCircleBitmap);
                    }
                }
            }
            else if (controlId == IDC_RADIO1 || controlId == IDC_RADIO2) {
                moves.clear();
                isCirclePlaced = false;
                lastSelectedRadioButton = nullptr;
                HBITMAP hEmptyBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
                for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
                    HWND hButton = GetDlgItem(hwnd, i);
                    SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hEmptyBitmap);
                }


                lastSelectedRadioButton = GetDlgItem(hwnd, controlId);
                isPlayerTurn = true;

                if (lastSelectedRadioButton == GetDlgItem(hwnd, IDC_RADIO2) && !isCirclePlaced) {
                    isCirclePlaced = true;
                    std::vector<int> availableButtons;
                    for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
                        if (IsMoveAvailable(i))
                            availableButtons.push_back(i);
                    }
                    if (!availableButtons.empty()) {
                        srand((unsigned int)time(NULL));
                        int randomIndex = rand() % availableButtons.size();
                        int buttonId = availableButtons[randomIndex];
                        HBITMAP hCircleBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
                        MakeMove(hwnd, buttonId, hCircleBitmap);
                    }
                }
            }
        }
        break;
    }

    }

    return FALSE;
}