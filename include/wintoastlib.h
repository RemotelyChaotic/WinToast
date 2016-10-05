#ifndef WINTOASTLIB_H
#define WINTOASTLIB_H
#include <windows.h>
#include <SDKDDKVer.h>
#include <WinUser.h>
#include <Shobjidl.h>
#include <wrl/implements.h>
#include <windows.ui.notifications.h>
#include <strsafe.h>
#include <Psapi.h>
#include <shlobj.h>
#include <roapi.h>
#include <propvarutil.h>
#include <functiondiscoverykeys.h>
#include <iostream>
#include <winstring.h>
#include <string.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Notifications;
using namespace Windows::Foundation;


typedef  ITypedEventHandler<ToastNotification*, ::IInspectable *>                                       ToastActivatedEventHandler;
typedef  ITypedEventHandler<ToastNotification*, ToastDismissedEventArgs *>                              ToastDismissedEventHandler;
typedef  ITypedEventHandler<ToastNotification*, ToastFailedEventArgs *>                                 ToastFailedEventHandler;
typedef Implements<ToastActivatedEventHandler, ToastDismissedEventHandler, ToastFailedEventHandler>     WinToastTemplateHandler;

#define DEFAULT_SHELL_LINKS_PATH	L"\\Microsoft\\Windows\\Start Menu\\Programs\\"
#define DEFAULT_LINK_FORMAT			L".lnk"

namespace WinToastLib {
    class WinToastStringWrapper {
    public:
        WinToastStringWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) throw();
        WinToastStringWrapper(_In_ const std::wstring &stringRef) throw();
        ~WinToastStringWrapper();
        HSTRING Get() const throw();
    private:
        HSTRING _hstring;
        HSTRING_HEADER _header;

    };


    class WinToastHandler : public WinToastTemplateHandler{
    public:
        enum WinToastDismissalReason {
            UserCanceled = ToastDismissalReason::ToastDismissalReason_UserCanceled,
            ApplicationHidden = ToastDismissalReason::ToastDismissalReason_ApplicationHidden,
            TimedOut = ToastDismissalReason::ToastDismissalReason_TimedOut
        };

        WinToastHandler(_In_ HWND hToActivate, _In_ HWND hEdit);
        WinToastHandler();
        ~WinToastHandler();
        virtual void toastActivated() const;
        virtual void toastDismissed(WinToastDismissalReason state) const;
        virtual void toastFailed() const;
    public:
        IFACEMETHODIMP_(ULONG) AddRef();
        IFACEMETHODIMP_(ULONG) Release();
    private:
        IFACEMETHODIMP Invoke(_In_ IToastNotification *toast, _In_ IInspectable *inspectable);
        IFACEMETHODIMP Invoke(_In_ IToastNotification *toast, _In_ IToastDismissedEventArgs *e);
        IFACEMETHODIMP Invoke(_In_ IToastNotification *toast, _In_ IToastFailedEventArgs *e);
        IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _COM_Outptr_ void **ppv);
    protected:
        ULONG _ref;
        HWND _hToActivate;
        HWND _hEdit;
    };

    class WinToastTemplate
    {
    public:
        enum WinToastTemplateType {
            ImageWithOneLine = ToastTemplateType::ToastTemplateType_ToastImageAndText01,
            ImageWithTwoLines = ToastTemplateType::ToastTemplateType_ToastImageAndText02,
            ImageWithThreeLines = ToastTemplateType::ToastTemplateType_ToastImageAndText03,
            ImageWithFourLines = ToastTemplateType::ToastTemplateType_ToastImageAndText04,
            TextOneLine = ToastTemplateType::ToastTemplateType_ToastText01,
            TextTwoLines = ToastTemplateType::ToastTemplateType_ToastText02,
            TextThreeLines = ToastTemplateType::ToastTemplateType_ToastText03,
            TextFourLines = ToastTemplateType::ToastTemplateType_ToastText04,
            UnknownTemplate = -1
        };

        WinToastTemplate(const WinToastTemplateType& type = ImageWithTwoLines);
        ~WinToastTemplate();
        virtual WinToastHandler* handler() const;

        int                                 textFieldsCount() const { return _textFieldsCount; }
        bool                                hasImage() const { return _hasImage; }
        std::vector<std::wstring>			textFields() const { return _textFields; }
        std::wstring                        textField(int pos) const { return _textFields[pos]; }
        std::wstring                        imagePath() const { return _imagePath; }
        WinToastTemplateType                type() const { return _type; }
        void                                setTextField(const std::wstring& txt, int pos);
        void                                setImagePath(const std::wstring& imgPath);
    private:
        int                                 _textFieldsCount;
        bool                                _hasImage;
        std::vector<std::wstring>			_textFields;
        std::wstring                        _imagePath;
        WinToastTemplateType                _type;
        void initComponentsFromType();
    };

    class WinToast {
    public:
        static WinToast* instance();
        static bool isCompatible();

        bool                initialize();
        bool                isInitialized() const { return _isInitialized; }
        bool                showToast(WinToastTemplate& toast);
        std::wstring        appName() const;
        std::wstring        appUserModelId() const;
        void                setAppUserModelId(_In_ const std::wstring& appName);
        void                setAppName(_In_ const std::wstring& appName);
    private:
        bool											_isInitialized;
        std::wstring                                    _appName;
        std::wstring                                    _aumi;
        ComPtr<IXmlDocument>                            _xmlDocument;
        ComPtr<IToastNotificationManagerStatics>        _notificationManager;
        ComPtr<IToastNotifier>                          _notifier;
        ComPtr<IToastNotificationFactory>               _notificationFactory;
        ComPtr<IToastNotification>                      _notification;
        static WinToast*								_instance;

        WinToast(void);
        IXmlDocument*							xmlDocument() const { return _xmlDocument.Get(); }
        IToastNotifier*							notifier() const { return _notifier.Get(); }
        IToastNotificationFactory*				notificationFactory() const { return _notificationFactory.Get(); }
        IToastNotificationManagerStatics*		notificationManager() const { return _notificationManager.Get(); }
        IToastNotification*						notification() const { return _notification.Get(); }

        HRESULT     validateShellLink();
        HRESULT		createShellLink();
        HRESULT		setImageField(_In_ const std::wstring& path);
        HRESULT     setTextField(_In_ const std::wstring& text, int pos);
    };



}

#endif // WINTOASTLIB_H
