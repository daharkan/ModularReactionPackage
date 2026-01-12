#ifndef EXPERIMENTRUNNERPOOL_H
#define EXPERIMENTRUNNERPOOL_H

#include <array>
#include <memory>

#include "ExperimentRunner.h"

class ExperimentRunnerPool
{
public:
    static ExperimentRunnerPool &instance();

    ExperimentRunner *runnerForPosition(int positionIndex);
    void resetRunner(int positionIndex);
    void resetAll();

private:
    ExperimentRunnerPool();

    static constexpr int kTotalPositions = 10;
    std::array<std::unique_ptr<ExperimentRunner>, kTotalPositions> m_runners;
};

#endif // EXPERIMENTRUNNERPOOL_H
