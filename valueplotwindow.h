#ifndef VALUEPLOTWINDOW_H
#define VALUEPLOTWINDOW_H

#include <QMainWindow>
#include "model.h"

/**
 * Group: Kimochi
 * Class: CS3505
 * Date: December 1, 2022
 * Project: A8 Educational App
 *
 * Reviewers: Shing Hei Ho (Adrian)
 *
 * This class plots the value (expected reward or likelihood of winning) from each game state onwards
 */
namespace Ui {
class ValuePlotWindow;
}

class ValuePlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ValuePlotWindow(QWidget *parent = nullptr);
    explicit ValuePlotWindow(Model& model, QWidget *parent = nullptr);
    ~ValuePlotWindow();

private:
    Ui::ValuePlotWindow *ui;
    std::map<std::vector<int>, float> valueMap_;

private slots:
    /**
     * @brief plot the values sepcified by valueMap
     * @param valueMap
     */
    void plotValueMapSlot(std::map<std::vector<int>, float> valueMap);

    /**
     * @brief plots values according to the number of aces specified by the user
     * @param numAce
     */
    void plotValuesChoiceSlot(int numAce);
};

#endif // VALUEPLOTWINDOW_H
