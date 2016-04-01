#ifndef __UISHADOW_H__
#define __UISHADOW_H__

#pragma once
#include "map"

namespace DuiLib
{

class UILIB_API CShadowUI
{
public:
    friend class CPaintManagerUI;

    CShadowUI(void);
    virtual ~CShadowUI(void);

public:
    typedef BOOL (WINAPI *pfnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT *pptDst,
            SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
    typedef HRESULT(WINAPI *pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);

    // bShowΪ��ʱ�Żᴴ����Ӱ
    void ShowShadow(bool bShow);
    bool IsShowShadow() const;

    // �㷨��Ӱ�ĺ���
    bool SetSize(int NewSize = 0);
    bool SetSharpness(unsigned int NewSharpness = 5);
    bool SetDarkness(unsigned int NewDarkness = 200);
    bool SetPosition(int NewXOffset = 5, int NewYOffset = 5);
    bool SetColor(COLORREF NewColor = 0);

    // ͼƬ��Ӱ�ĺ���
    bool SetImage(LPCTSTR szImage);
    bool SetCorner(RECT rcCorner);  // �Ź���ʽ������Ӱ

    int GetSize() const
    {
        return m_nSize;
    }
    int GetSharpness() const
    {
        return m_nSharpness;
    }
    int GetDarkness() const
    {
        return m_nDarkness;
    }
    POINT GetPosition() const
    {
        POINT pt = {m_nxOffset, m_nyOffset};
        return pt;
    };
    COLORREF GetColor() const
    {
        return m_Color;
    }

    // ͼƬ��Ӱ�ĺ���
    LPCTSTR GetImage() const
    {
        return m_sShadowImage.c_str();
    }
    RECT GetCorner() const
    {
        return m_rcShadowCorner;   // �Ź���ʽ������Ӱ
    }
    // ���Լ�����Ӱ��ʽ���Ƶ��������
    bool CopyShadow(CShadowUI* pShadow);

    //  ������Ӱ���壬��CPaintManagerUI�Զ�����,�����Լ�Ҫ����������Ӱ
    void Create(HWND hWndParent, CPaintManagerUI* pPaintManager);

    //  ���໯������
    LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:

    //  ��ʼ����ע����Ӱ��
    static bool Initialize(HINSTANCE hInstance);

    // �����Ѿ����ӵĴ������������������Ӱ��,������ParentProc()������ͨ������õ���Ӱ��
    static std::map<HWND, CShadowUI *>& GetShadowMap();



    // ������ı��С���ƶ������������ػ���Ӱʱ����
    void Update(HWND hParent);

    // ͨ���㷨������Ӱ
    void MakeShadow(UINT32 *pShadBits, HWND hParent, RECT *rcParent);

    // ����alphaԤ��ֵ
    inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
    {
        return (GetRValue(cl) * (DWORD)nAlpha / 255) |
               (GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
               (GetBValue(cl) * (DWORD)nAlpha / 255) << 16 ;
    }

    // Show or hide the shadow, depending on the enabled status stored in m_Status
    void Show(HWND hParent);
protected:
    enum ShadowStatus {
        SS_ENABLED = 1,             // Shadow is enabled, if not, the following one is always false
        SS_VISABLE = 1 << 1,        // Shadow window is visible
        SS_PARENTVISIBLE = 1 << 2,  // Parent window is visible, if not, the above one is always false
        SS_DISABLEDBYAERO   = 1 << 3    // Shadow is enabled, but do not show because areo is enabled
    };
    void SetStatus(int state);
    // Layered window APIs

    /*  static std::map<HWND, CShadowUI *> *s_Shadowmap;*/
    static bool s_bHasInit;
    // Vista compatibility APIs
    static bool s_bVista;   // Whether running on Win Vista
    static pfnUpdateLayeredWindow       s_UpdateLayeredWindow;
    static pfnDwmIsCompositionEnabled   s_DwmIsCompositionEnabled;



    // �㷨��Ӱ��Ա����

    // The X and Y offsets of shadow window,
    // relative to the parent window, at center of both windows (not top-left corner), signed


    // Restore last parent window size, used to determine the update strategy when parent window is resized
    LPARAM m_WndSize;
    // Set this to true if the shadow should not be update until next WM_PAINT is received
    bool m_bUpdate;
    COLORREF m_Color;   // Color of shadow

    // ͼƬ��Ӱ��Ա����


private:
    unsigned char m_nDarkness;  // Darkness, transparency of blurred area
    unsigned char m_nSharpness; // Sharpness, width of blurred border of shadow window
    signed char m_nSize;    // Shadow window size, relative to parent window size

    CPaintManagerUI *m_pManager;        // �������CPaintManagerUI��������ȡ�ز���Դ�͸�������
    HWND             m_hWnd;            // ��Ӱ����ľ��
    LONG             m_OriParentProc;   // ���໯������
    BYTE             m_Status;
    bool             m_bIsImageMode;    // �Ƿ�ΪͼƬ��Ӱģʽ
    bool             m_bIsShowShadow;   // �Ƿ�Ҫ��ʾ��Ӱ
    RECT            m_rcShadowCorner;
    std::wstring    m_sShadowImage;
    signed char m_nxOffset;
    signed char m_nyOffset;
};

}

#endif //__UISHADOW_H__