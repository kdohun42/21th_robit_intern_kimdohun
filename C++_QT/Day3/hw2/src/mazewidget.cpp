#include "mazewidget.h"

#include <QPainter>

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent)
{
}

void MazeWidget::setMaze(
    const std::vector<std::vector<int>>& maze
    )
{
    m_maze = maze;

    m_visited.clear();
    m_path.clear();

    update();
}

void MazeWidget::setMapData(
    const std::vector<std::vector<int>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end
    )
{
    m_maze = maze;

    m_start = start;
    m_end = end;

    m_visited.clear();
    m_path.clear();

    update();
}

void MazeWidget::setVisited(
    const std::vector<std::vector<bool>>& visited
    )
{
    m_visited = visited;

    update();
}

void MazeWidget::setPath(
    const std::vector<std::pair<int, int>>& path
    )
{
    m_path = path;

    update();
}

void MazeWidget::setMode(Mode mode)
{
    m_mode = mode;
}

void MazeWidget::resizeMaze(int rows, int cols)
{
    m_maze.assign(
        rows,
        std::vector<int>(cols, 0)
        );

    m_visited.clear();
    m_path.clear();

    m_start = {0, 0};

    m_end = {
        rows - 1,
        cols - 1
    };

    update();
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_maze.empty() ||
        m_mode == Mode::None)
    {
        return;
    }

    int rows = m_maze.size();

    int cols = m_maze[0].size();

    int cellW = width() / cols;

    int cellH = height() / rows;

    int y =
        event->pos().x() / cellW;

    int x =
        event->pos().y() / cellH;

    if (x < 0 ||
        x >= rows ||
        y < 0 ||
        y >= cols)
    {
        return;
    }

    if (m_mode == Mode::Obstacle)
    {
        if (std::make_pair(x, y) == m_start ||
            std::make_pair(x, y) == m_end)
        {
            return;
        }

        m_maze[x][y] =
            (m_maze[x][y] == 1)
                ? 0
                : 1;
    }

    else if (m_mode == Mode::Start)
    {
        if (m_maze[x][y] == 1)
        {
            return;
        }

        m_start = {x, y};
    }

    else if (m_mode == Mode::End)
    {
        if (m_maze[x][y] == 1)
        {
            return;
        }

        m_end = {x, y};
    }

    m_visited.clear();
    m_path.clear();

    update();

    emit mazeEdited();
}

void MazeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (m_maze.empty())
    {
        return;
    }

    int rows = m_maze.size();

    int cols = m_maze[0].size();

    int cellW =
        width() / cols;

    int cellH =
        height() / rows;

    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            QRect cellRect(
                y * cellW,
                x * cellH,
                cellW,
                cellH
                );

            // 장애물
            if (m_maze[x][y] == 1)
            {
                painter.fillRect(
                    cellRect,
                    Qt::black
                    );
            }

            // 방문한 노드
            else if (
                !m_visited.empty() &&
                m_visited[x][y]
                )
            {
                painter.fillRect(
                    cellRect,
                    Qt::green
                    );
            }

            // 일반 길
            else
            {
                painter.fillRect(
                    cellRect,
                    Qt::white
                    );
            }

            painter.setPen(Qt::gray);

            painter.drawRect(cellRect);
        }
    }

    // 시작점
    QRect startRect(
        m_start.second * cellW,
        m_start.first * cellH,
        cellW,
        cellH
        );

    painter.fillRect(
        startRect,
        Qt::red
        );

    // 도착점
    QRect endRect(
        m_end.second * cellW,
        m_end.first * cellH,
        cellW,
        cellH
        );

    painter.fillRect(
        endRect,
        Qt::yellow
        );

    // 최단 경로
    for (auto& p : m_path)
    {
        QRect cellRect(
            p.second * cellW,
            p.first * cellH,
            cellW,
            cellH
            );

        painter.fillRect(
            cellRect,
            Qt::blue
            );
    }
}
