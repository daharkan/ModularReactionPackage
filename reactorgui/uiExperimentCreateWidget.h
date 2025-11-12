#ifndef UIEXPERIMENTCREATEWIDGET_H
#define UIEXPERIMENTCREATEWIDGET_H

#include <QWidget>
#include "qlineedit.h"
#include "qpushbutton.h"
#include "uiExperimentGraph.h"

#define PAGE_CREATE_IDX                0
#define PAGE_WARNING_IDX               1
#define PAGE_ADVANCED_IDX              2


enum ExperimentType{
    EXP_UNDEFINED,
    EXP_BASIC_STANDARD,
    EXP_BASIC_LI_UP_ST_DOWN,
    EXP_BASIC_ST_UP_LI_DOWN,
    EXP_BASIC_ST_UP_ST_DOWN,
    EXP_ADVANCED_LIN,
    EXP_ADVANCED_PLATUE,
    EXP_ADVANCED_STEPS,
    EXP_ADVANCED_2NDDEG
};


enum TempComboBoxIndex{
    IDX_TEMP_CELCIUS = 0,
    IDX_TEMP_FAHRENHEIT = 1
};

enum TimeomboBoxIndex{
    IDX_TIME_SEC = 0,
    IDX_TIME_MIN = 1,
    IDX_TIME_HOUR = 2
};



namespace Ui {
class ExperimentCreateWidget;
}

class ExperimentCreateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExperimentCreateWidget(QWidget *parent = nullptr);
    ~ExperimentCreateWidget();

private:
    Ui::ExperimentCreateWidget *ui;
    ExperimentGraph *m_expGraph = nullptr;
    Experiment m_currentExperiment;
    ExperimentType m_currentExpType = EXP_UNDEFINED;
    QPushButton* m_lastClickedButton = nullptr;
    int m_lastPageIndex = 0;

    void setVisibleAllBasicExperimentItems(bool en);
    void setVisibleAllAdvExperimentItems(bool en);
    void adv_forceInitialTemp();

    Profile createBasicStandardProfile();
    Profile createLinearUpStepDownProfile();
    Profile createStepUpLinearDownProfile();
    Profile createStepUpStepDownProfile();

    void blinkLineEdit(QLineEdit* lineEdit, int blinkCount = 3);


    TempArc createLinearTempArc(float startTemp, float endTemp, float rank, unsigned long startTimeMSec);
    TempArc createPlateuTempArc(float startTemp, unsigned long startTimeMSec, unsigned long durationMSec);
    std::vector<TempArc> createStepTempArc(float startTemp, float endTemp, int stepCount, unsigned long stepUpDownDuration, unsigned long startTimeMSec);
    TempArc create2ndDegArc(float startTemp, float endTemp, float a, unsigned long startTimeMSec, unsigned long durationMSec);




private slots:
    void handleBasicsButtonClicked();
    void advancedProfileButtonClicked();
    void updateClicked();

    void adv_addPlatueClicked();
    void adv_addLinearClicked();
    void adv_addStepsClicked();
    void adv_add2ndDegClicked();
    void adv_addArcToExperimentClicked();
    void adv_delLastArcClicked();
    void adv_clearProfileClicked();

    void adv_addArcDurationChanged();
    void adv_rampChanged();



};

#endif // UIEXPERIMENTCREATEWIDGET_H
