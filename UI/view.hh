/* file: view.hh
 * description: Header for class View. Class is used for implementing zoom
 * and panning to the view.
 */

#ifndef VIEW_HH
#define VIEW_HH

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>

namespace Student {

class View : public QGraphicsView
{
public:
    /**
     * @brief Constructor, sets some parameters for the graphicsView.
     */
    View();
    ~View() = default;

protected:
    /**
     * @brief wheelEvent Zooms the view when scrolling with mouse wheel.
     */
    void wheelEvent(QWheelEvent *event);

    /**
     * @brief mousePressEvent Starts moving the view if rightclick is pressed.
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent Stops moving the view when rightclick is released.
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief mouseMoveEvent Moves the view with mouse.
     */
    void mouseMoveEvent(QMouseEvent *event);

private:
    //! Data needed for moving the view is stored here.
    bool pan_;
    int panStartX_;
    int panStartY_;
};

}
#endif // VIEW_HH
