#include "valueplotwindow.h"
#include "ui_valueplotwindow.h"

ValuePlotWindow::ValuePlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ValuePlotWindow)
{
    this->setFixedSize(860, 615);
    ui->setupUi(this);
}

ValuePlotWindow::ValuePlotWindow(Model& model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ValuePlotWindow)
{
    ui->setupUi(this);

    //set up number of ace selection for plotting
    for(unsigned int i=0; i<5; i++){
        ui->numAceChoiceBox->addItem(QString::number(i));
    }


    //setup plot
    ui->valuePlot0->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    ui->valuePlot0->axisRect()->setupFullAxesBox(true);
    ui->valuePlot0->xAxis->setLabel("dealer's revealed card");
    ui->valuePlot0->yAxis->setLabel("sum of card value excluding ace in player's hand");

    //for plotting values
    connect(&model,
            &Model::plotValueMapSignal,
            this,
            &ValuePlotWindow::plotValueMapSlot);

    connect(ui->numAceChoiceBox,
            &QComboBox::currentIndexChanged,
            this,
            &ValuePlotWindow::plotValuesChoiceSlot);

}

ValuePlotWindow::~ValuePlotWindow()
{
    delete ui;
}

void ValuePlotWindow::plotValuesChoiceSlot(int numAce){

    // remove and set up the QCPColorMap:
    ui->valuePlot0->clearPlottables();
    QCPColorMap *colorMap = new QCPColorMap(ui->valuePlot0->xAxis, ui->valuePlot0->yAxis);

    int nx = 11;
    int ny = 31;
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(1, 11), QCPRange(1, 31)); // and span the coordinate range in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    for(const auto& pair:valueMap_){
        if(pair.first[2]==numAce){
            int playerValueNoAce = pair.first[0];
            int dealerCard = pair.first[1];
            colorMap->data()->setCell(dealerCard, playerValueNoAce, pair.second);
        }
    }

    //remove and set up color scale
    int elementCount = ui->valuePlot0->plotLayout()->elementCount();
    for(int i = 0; i < elementCount; i++){
        if(qobject_cast<QCPColorScale*>(ui->valuePlot0->plotLayout()->elementAt(i)))
                ui->valuePlot0->plotLayout()->removeAt(i);

        //collapse the empty elements
       ui->valuePlot0->plotLayout()->simplify();
    }

    ui->valuePlot0->replot();

    // add a color scale
    QCPColorScale *colorScale = new QCPColorScale(ui->valuePlot0);
    colorScale->setDataRange(QCPRange(-1, 1));
    ui->valuePlot0->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("expected reward from the state");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpPolar);

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->valuePlot0);
    ui->valuePlot0->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    ui->valuePlot0->rescaleAxes();
}

void ValuePlotWindow::plotValueMapSlot(std::map<std::vector<int>, float> valueMap){

    this->valueMap_ = valueMap;
}

