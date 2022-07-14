#ifndef PAINTING_TOOLS_H
#define PAINTING_TOOLS_H

#include <qwt_plot.h>

namespace insight {
namespace graphic {



int painter_coord_from_axis_scale(QwtPlot *, double, int);
double axes_coord_from_painter_scale(QwtPlot *, int, int);

int painter_coordx_from_axis_scale(QwtPlot *, double);
int painter_coordy_from_axis_scale(QwtPlot *, double);

double axis_coordx_from_painter_scale(QwtPlot *, int);
double axis_coordy_from_painter_scale(QwtPlot *, int);

}  // namespace graphic
}  // namespace insight
#endif // PAINTING_TOOLS_H
