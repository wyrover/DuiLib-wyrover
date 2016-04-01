/*!
* \file Animator.h
* \brief �������
*
* �����������
*
* \author wangyang
* \date 2015/02/15
* \version 1.0
*/

/*
Example:

void do_animator()
{
    TThreadMethod update_callback = boost::bind(&CMainFrm::animator_update, this);
    TThreadMethod complate_callback = boost::bind(&CMainFrm::animator_complate, this);
    animator_.reset(new DuiLib::Animator());
    animator_->set_update_callback(update_callback);
    animator_->set_complete_callback(complate_callback);
    animator_->set_elapsed(1000);
    animator_->set_start_value(pCon->GetPos().top);
    animator_->set_end_value(100);
    animator_->set_tweener(tween::BACK, tween::EASE_IN_OUT);
    animator_->start();
}


void CMainFrm::animator_update()
{
    RECT rect;
    CContainerUI* pCon = reinterpret_cast<CContainerUI*>(m_PaintManager.FindControl(_T("MainFrameOpBtnView")));     rect = pCon->GetPos();
    rect.top = animator_->get_value();
    pCon->SetPos(rect);
}

void CMainFrm::animator_complate()
{
    CContainerUI* pCon = reinterpret_cast<CContainerUI*>(m_PaintManager.FindControl(_T("MainFrameOpBtnView")));
    m_PaintManager.SendNotify(pCon, DUI_MSGTYPE_CHANGEMAINPAGE);
}
*/

#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include "DuiLib/Animation/CppTweener.h"
#include <cactus/cactus_pattern.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace cactus;
using namespace tween;

namespace DuiLib
{
/*! @brief tween�����ӿ�
 *
 * tween�����ӿ�
 */
class IAnimator
{
public:
    virtual ~IAnimator() { }
    /*! @brief ������ʼ
     *
     * @return ���ؿ�
     */
    virtual void start() = 0;
    /*! @brief ������ͣ
     *
     * @return ���ؿ�
     */
    virtual void pause() = 0;
    /*! @brief ����ȡ��
     *
     * @return ���ؿ�
     */
    virtual void cancel() = 0;
    /*! @brief ������ֹ
     *
     * @return ���ؿ�
     */
    virtual void end() = 0;
    /*! @brief �Ƿ���������
     *
     * @return ����true��ʾ�ɹ�������Ϊʧ��
     */
    virtual bool is_running() = 0;
    /*! @brief ����tween����
     *
     * @param[in] quart ����˵��
     * @param[in] easy ����˵��
     * @return ���ؿ�
     */
    virtual void set_tweener(int quart, int easy) = 0;
    /*! @brief ���ö���ѭ������
     *
     * @param[in] count ����ѭ������
     * @return ���ؿ�
     */
    virtual void set_repeat_count(int value = -1) = 0;
    /*! @brief ���ö�������
     *
     * @param[in] value �����Ƿ���
     * @return ���ؿ�
     */
    virtual void set_reverse(bool value) = 0;
    /*! @brief ���ö�����ʱ
     *
     * @param[in] value ��ʱ ����
     * @return ���ؿ�
     */
    virtual void set_interval(float value) = 0;
    /*! @brief ���ó�ʼֵ
     *
     * @param[in] value ��ʼֵ
     * @return ���ؿ�
     */
    virtual void set_start_value(double value) = 0;
    /*! @brief �����ս�ֵ
     *
     * @param[in] value �ս�ֵ
     * @return ���ؿ�
     */
    virtual void set_end_value(double value) = 0;
    /*! @brief ��ȡ��ǰ����ֵ
     *
     * @return ���ص�ǰ����ֵ
     */
    virtual double get_value() = 0;
    /*! @brief ���ö������лص�����
     *
     * @param[in] update_callback ���»ص�����
     * @return ���ؿ�
     */
    virtual void set_update_callback(boost::function<void()> update_callback) = 0;
    /*! @brief ���ö�����ɻص�����
     *
     * @param[in] complete_callback ������ɻص�����
     * @return ���ؿ�
     */
    virtual void set_complete_callback(boost::function<void()> complete_callback) = 0;

    virtual void update(long currentMillis) = 0;
    virtual void set_name(LPCTSTR name) = 0;
};

class UILIB_API Animator : public IAnimator, TweenerListener
{
public:
    Animator();
    virtual ~Animator();
public:
    void start();
    void cancel();
    void end();
    void pause();
    void resume();
    bool is_paused();
    bool is_running();
    //bool is_complated();
    bool is_started();
    void set_tweener(int quart, int easy);
    void set_repeat_count(int value = -1);
    void set_reverse(bool value);
    void set_interval(float speed);
    void set_start_value(double value);
    void set_end_value(double value);
    void set_name(LPCTSTR name);
    double get_value();
    void set_update_callback(boost::function<void()> update_callback);
    void set_complete_callback(boost::function<void()> complete_callback);
    void update(long currentMillis);
public:
    void onStart(const TweenerParam& param);
    void onStep(const TweenerParam& param);
    void onComplete(const TweenerParam& param);
private:
    //void _run();
protected:
    bool paused_;
    bool running_;
    //bool complated_;
private:
    boost::shared_ptr<boost::thread>    thrd_;
    boost::recursive_mutex                        mutex_;
    //boost::condition_variable           condition_;
    boost::function<void()>                       update_callback_;
    boost::function<void()>                       complate_callback_;
    tween::Tweener                      tweener_;
    float                               speed_;
    double                              start_pos_;
    double                              lost_pos_;
    double                              new_pos_;
    int                                 quart_;
    int                                 easy_;
    int                                 repeat_;
    int                                 reverse_;
    std::wstring                        name_;
};

//typedef boost::shared_ptr<IAnimator> IAnimatorPtr;
typedef IAnimator* IAnimatorPtr;

class AnimatorManagerImpl
{
public:
    AnimatorManagerImpl();
    ~AnimatorManagerImpl();
public:
    void add_animator(IAnimatorPtr animator);
    void remove_animator(IAnimatorPtr animator);
    bool exist(IAnimatorPtr animator);
private:
    void _run();
private:
    std::vector<IAnimatorPtr>           animator_list_;
    boost::shared_ptr<boost::thread>    thrd_;
    boost::recursive_mutex                        mutex_;
    boost::mutex                        con_mutex_;
    boost::condition_variable           condition_;
};

typedef cactus::Singleton<AnimatorManagerImpl> AnimatorManager;

} // namespace DuiLib



#endif // __ANIMATOR_H__
