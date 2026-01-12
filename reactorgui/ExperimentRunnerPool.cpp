#include "ExperimentRunnerPool.h"

ExperimentRunnerPool &ExperimentRunnerPool::instance()
{
    static ExperimentRunnerPool pool;
    return pool;
}

ExperimentRunnerPool::ExperimentRunnerPool()
{
    for (int i = 0; i < kTotalPositions; ++i) {
        m_runners[i] = std::make_unique<ExperimentRunner>();
    }
}

ExperimentRunner *ExperimentRunnerPool::runnerForPosition(int positionIndex)
{
    if (positionIndex < 1 || positionIndex > kTotalPositions) {
        return nullptr;
    }
    return m_runners[positionIndex - 1].get();
}

void ExperimentRunnerPool::resetRunner(int positionIndex)
{
    ExperimentRunner *runner = runnerForPosition(positionIndex);
    if (runner == nullptr) {
        return;
    }
    runner->reset();
}

void ExperimentRunnerPool::resetAll()
{
    for (int i = 1; i <= kTotalPositions; ++i) {
        resetRunner(i);
    }
}
