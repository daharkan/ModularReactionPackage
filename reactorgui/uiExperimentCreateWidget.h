#ifndef UIEXPERIMENTCREATEWIDGET_H
#define UIEXPERIMENTCREATEWIDGET_H

#include <QWidget>
#include <string>
#include "qlineedit.h"
#include "qpushbutton.h"
#include "uiExperimentGraph.h"
#include "User.h"

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

class QButtonGroup;

class ExperimentCreateWidget : public QWidget
{
    Q_OBJECT

public:
    enum class Mode {
        Create,
        Edit,
        Show
    };

    explicit ExperimentCreateWidget(QWidget *parent = nullptr);
    ~ExperimentCreateWidget();
    void loadExperiment(const Experiment &experiment, Mode mode);
    void setCurrentUser(const User &user);

private:
    enum class Adv2ndDegDriver {
        None,
        A,
        Target
    };

    Ui::ExperimentCreateWidget *ui;
    ExperimentGraph *m_expGraph = nullptr;
    Experiment m_currentExperiment;
    ExperimentType m_currentExpType = EXP_UNDEFINED;
    Mode m_mode = Mode::Create;
    User m_currentUser;
    QPushButton* m_lastClickedButton = nullptr;
    int m_lastPageIndex = 0;
    QPushButton *m_assignButton = nullptr;
    QPushButton *m_assignButtonAdvanced = nullptr;
    Adv2ndDegDriver m_adv2ndDegDriver = Adv2ndDegDriver::None;
    QButtonGroup *m_advArcButtonGroup = nullptr;

    void setVisibleAllBasicExperimentItems(bool en);
    void updateBasicGroupVisibility();
    void setVisibleAllAdvExperimentItems(bool en, bool clearFields = true);
    void adv_forceInitialTemp();
    void adv_update2ndDegBindings(Adv2ndDegDriver driver);
    void clearAdvancedArcSelection();
    void updatePreviewFromUi();
    bool validateBasicInputs(bool showErrors);

    Profile createBasicStandardProfile();
    Profile createLinearUpStepDownProfile();
    Profile createStepUpLinearDownProfile();
    Profile createStepUpStepDownProfile();

    void blinkLineEdit(QLineEdit* lineEdit, int blinkCount = 3);
    bool buildProfile(Profile &profile);
    void applyExperimentType(ExperimentType type);
    void updateModeUi();
    std::string promptExperimentName(const QString &currentName);
    std::string captureUiState() const;
    void applyUiState(const std::string &stateJson);
    void assignExperimentToCells();
    void setInputsEnabled(bool enabled);


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
    void adv_polyAChanged();
    void adv_finalTempChanged();

signals:
    void sgn_experimentSaved();



};

#endif // UIEXPERIMENTCREATEWIDGET_H
