# Bug Repro

This is a minimal repro for a bug in CsWinRT where, if the WinRT.Host.dll were to asked to activate a class that doesn't exist, or at least is not a cswinrt component, subsequent call to activate class that does exist would crash.

## Steps to reproduce
1. Build CsWinRTHostThrow component.
2. Build and launch App1. Click the button and see CsWinRTHost's xaml provider instansiated
2. Open MainWindow.cpp in App1, uncomment a line which, before activating CsWinRTHost's xaml provider, tries to activate a class with bogus name, according to the comment
3. Build and launch App1 again. Click the button and observe crash
