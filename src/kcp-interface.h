#ifndef KCP_INTERFACE_H
#define KCP_INTERFACE_H
#include <kiran-control-panel/kcp-plugin-interface.h>

class QTranslator;
class DbusWrapper;
class KcpInterface : public QObject, public KcpPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID KcpPluginInterface_iid)
    Q_INTERFACES(KcpPluginInterface)

public:
    KcpInterface();
    ~KcpInterface(){};

public:
    /**
     * 插件需提供的初始化方法，在其中加载翻译文件或做其他初始化操作
     * \return 初始化返回值 返回0标志成功，其他值标志失败！
     */
    virtual int init() override;

    /**
     * 插件需提供取消初始化的操作，在其中对翻译文件进行卸载或取消其他初始化操作
     */
    virtual void uninit() override;

    /**
     * \brief 通过插件功能项名称(PluginSubItem->name)获取显示控件
     * \param id 功能项ID
     * \return 该功能项的显示控件
     */
    virtual QWidget* getSubItemWidget(QString subItemName) override;

    /**
     * 插件实现该方法用于判断是否存在未保存的设置项,用于提供切换页面时做检查
     * \return 是否存在未保存项
     */
    virtual bool haveUnsavedOptions() override;

    QStringList visibleSubItems() override;

private:
    QWidget* m_currentWidget = nullptr;
    QTranslator* m_translator = nullptr;
    DbusWrapper* m_dbusWrapper = nullptr;
};

#endif  // KCP_INTERFACE_H
