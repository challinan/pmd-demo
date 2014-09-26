
#include <QWidget>
#include "data.h"

namespace Ui {
    class GraphSettingsPopup;
}

class MainWindow;

class GraphSettingsPopup : public QWidget
{
    Q_OBJECT

public:
    explicit GraphSettingsPopup(MainWindow *parent = 0);
    ~GraphSettingsPopup();
    void initialized(int graphSize,TGraphType graphType );

private slots:
    void on_pbtn_SizeMinus_clicked();
    void on_pbtn_SizeAuto_clicked();
    void on_pbtn_SizePlus_clicked();
    void on_pbtn_wave_co2_clicked();
    void on_pbtn_wave_abp_clicked();
    void on_pbtn_wave_ecg_clicked();
    void on_pbtn_wave_pleth_clicked();

public:
    int m_graphWaveSize;
    TGraphType m_graphType;
    bool m_isVisible;

private:
    Ui::GraphSettingsPopup *ui;
    MainWindow* m_mainWindow;
};
