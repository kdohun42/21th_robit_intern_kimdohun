#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include <vector>
#include <utility>

class MazeWidget : public QWidget
{
    Q_OBJECT

public:

    enum class Mode
    {
        None,
        Obstacle,
        Start,
        End
    };

    explicit MazeWidget(QWidget *parent = nullptr);

    void setMaze(
        const std::vector<std::vector<int>>& maze
        );

    void setMapData(
        const std::vector<std::vector<int>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end
        );

    void setVisited(
        const std::vector<std::vector<bool>>& visited
        );

    void setPath(
        const std::vector<std::pair<int, int>>& path
        );

    void setMode(Mode mode);

    void resizeMaze(int rows, int cols);

    std::vector<std::vector<int>> getMaze() const
    {
        return m_maze;
    }

    std::pair<int, int> getStart() const
    {
        return m_start;
    }

    std::pair<int, int> getEnd() const
    {
        return m_end;
    }

signals:

    void mazeEdited();

protected:

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:

    std::vector<std::vector<int>> m_maze;

    std::vector<std::vector<bool>> m_visited;

    std::vector<std::pair<int, int>> m_path;

    Mode m_mode = Mode::None;

    std::pair<int, int> m_start{0, 0};

    std::pair<int, int> m_end{0, 0};
};

#endif
