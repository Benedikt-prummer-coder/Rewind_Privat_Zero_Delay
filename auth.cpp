char buf[256]{};

DialogBoxParamA(
    nullptr,
    MAKEINTRESOURCEA(101),
    nullptr,
    [](HWND hwnd, UINT msg, WPARAM w, LPARAM l) -> INT_PTR
    {
        static char* buffer = nullptr;

        if (msg == WM_INITDIALOG)
        {
            buffer = reinterpret_cast<char*>(l);
            return TRUE;
        }

        if (msg == WM_COMMAND && LOWORD(w) == IDOK)
        {
            GetDlgItemTextA(hwnd, 1001, buffer, 256);
            EndDialog(hwnd, IDOK);
            return TRUE;
        }
        return FALSE;
    },
    reinterpret_cast<LPARAM>(buf)
);
