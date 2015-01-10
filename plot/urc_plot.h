#ifndef URC_PLOT_H
#define URC_PLOT_H

#include <Library/qcustomplot/qcustomplot.h>
#include <QTimer>

#define RemoveGraphCode 0

class URC_Plot :public QCustomPlot
{
  Q_OBJECT

public:
  explicit URC_Plot(QWidget *parent =0);
  ~URC_Plot();

  void setup_URC_vel_plot();
  void URC_vel_data_update(float velx, float vely);
#if RemoveGraphCode
  void setupDemo(int demoIndex);
  void setupQuadraticDemo(QCustomPlot *customPlot);
  void setupSimpleDemo(QCustomPlot *customPlot);
  void setupSincScatterDemo(QCustomPlot *customPlot);
  void setupScatterStyleDemo(QCustomPlot *customPlot);
  void setupLineStyleDemo(QCustomPlot *customPlot);
  void setupScatterPixmapDemo(QCustomPlot *customPlot);
  void setupDateDemo(QCustomPlot *customPlot);
  void setupTextureBrushDemo(QCustomPlot *customPlot);
  void setupMultiAxisDemo(QCustomPlot *customPlot);
  void setupLogarithmicDemo(QCustomPlot *customPlot);
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void setupParametricCurveDemo(QCustomPlot *customPlot);
  void setupBarChartDemo(QCustomPlot *customPlot);
  void setupStatisticalDemo(QCustomPlot *customPlot);
  void setupSimpleItemDemo(QCustomPlot *customPlot);
  void setupItemDemo(QCustomPlot *customPlot);
  void setupStyledDemo(QCustomPlot *customPlot);
  void setupAdvancedAxesDemo(QCustomPlot *customPlot);

  void setupPlayground(QCustomPlot *customPlot);
#endif

  QFont font(){return QFont();}
private slots:
#if RemoveGraphCode
  void realtimeDataSlot();
  void bracketDataSlot();
#endif
//  void screenShot();
//  void allScreenShots();


private:
  QCustomPlot *customPlot;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
};
#endif // URC_PLOT_H
