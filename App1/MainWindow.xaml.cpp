#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include "wil/resource.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::App1::implementation
{
    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        std::wstring path{ L"CsWinRTHostThrow.BlankPage1" };
        std::size_t count{};

        while (std::wstring::npos != (count = path.rfind('.')))
        {
            path.resize(count);
            auto aNamespace = path;
            path += L".dll";
            wil::unique_hmodule library((HMODULE)winrt::impl::load_library(L"WinRT.Host.dll"));
            path.resize(path.size() - 4);

            if (!library)
            {
                continue;
            }

            auto library_call = reinterpret_cast<int32_t(__stdcall*)(void* classId, void** factory)>(WINRT_IMPL_GetProcAddress(library.get(), "DllGetActivationFactory"));

            if (!library_call)
            {
                continue;
            }

            auto loader = [&library_call](winrt::param::hstring const& name) -> Windows::Foundation::IActivationFactory
                {
                    Windows::Foundation::IActivationFactory library_factory;
                    if (0 != library_call(*(void**)(&name), winrt::put_abi(library_factory)))
                    {
                        return nullptr;
                    }
                    return library_factory;
                };
            auto bogusName{ winrt::hstring(L"Microsoft.UI.Xaml.Controls") };
            auto expectedName{ winrt::hstring(aNamespace) + L"." + aNamespace + L"_XamlTypeInfo.XamlMetaDataProvider" };
            Windows::Foundation::IActivationFactory factory;
            // Uncomment to reproduce bug
            //factory = loader(bogusName);
            if (factory == nullptr)
            {
                factory = loader(expectedName);
            }
            if (factory == nullptr)
            {
                continue;
            }

            auto provider = factory.ActivateInstance<winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider>();
            if (provider != nullptr)
            {
                myButton().Content(box_value(L"Instantiated cswinrt xaml provider"));
            }
        }
    }
}
