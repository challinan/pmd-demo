
#include <QWidget>
#include "data.h"

namespace Ui {
class EngMenuPopup;
}

class MainWindow;

class EngMenuPopup : public QWidget
{
    Q_OBJECT

public:
    explicit EngMenuPopup(MainWindow *parent = 0);
    ~EngMenuPopup();
    void initialized(bool scrolValue);

    bool isScrollingStateChanged(bool &scrollingState);
    bool isNucleusStateChanged(bool &nucleusState);

private slots:
    void on_btn_Nucleus_clicked(bool checked);

    void on_btn_Scrolling_clicked(bool checked);

public:
    int m_graphWaveSize;
    TGraphType m_graphType;
    bool m_isVisible;

private:
    Ui::EngMenuPopup *ui;
    MainWindow* m_mainWindow;
    bool m_nucluesStatus; //true if started false if not
    bool m_scrollingStatus; //true if enabled false if not
    bool m_previousNucleusState;
    bool m_previousScrollingStatus;
};
