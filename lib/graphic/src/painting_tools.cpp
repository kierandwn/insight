#include <qwt_plot.h>

namespace insight {
namespace graphic {


int painter_coord_from_axis_scale(QwtPlot * plot_obj, double coordinate, int axis_id)
{
    QwtScaleMap map = plot_obj->canvasMap(axis_id);
    return map.transform(coordinate);
}

double axes_coord_from_painter_scale(QwtPlot * plot_obj, int coordinate, int axis_id)
{
    QwtScaleMap map = plot_obj->canvasMap(axis_id);
    return map.invTransform(coordinate);
}

int painter_coordx_from_axis_scale(QwtPlot * plot_obj, double coordinate_x)
{
    return painter_coord_from_axis_scale(plot_obj, coordinate_x, QwtPlot::Axis::xBottom);
}
int painter_coordy_from_axis_scale(QwtPlot * plot_obj, double coordinate_y)
{
    return painter_coord_from_axis_scale(plot_obj, coordinate_y, QwtPlot::Axis::yLeft);
}

double axis_coordx_from_painter_scale(QwtPlot * plot_obj, int coordinate_x)
{
    return axes_coord_from_painter_scale(plot_obj, coordinate_x, QwtPlot::Axis::xBottom);
}
double axis_coordy_from_painter_scale(QwtPlot * plot_obj, int coordinate_y)
{
    return axes_coord_from_painter_scale(plot_obj, coordinate_y, QwtPlot::Axis::yLeft);
}


}  // namespace graphic
}  // namespace insight



