#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <QAbstractListModel>
#include <vector>

enum class STATE {
    NOTHING_PRESSED = 0,
    FIRST_PRESSED,
    SECOND_PRESSED
};

class GameArea : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)

public:
//    static constexpr size_t INVISIBLE { -1 };
    static constexpr size_t defaultDimension { 4 };

    GameArea(size_t dimension = defaultDimension, QObject* parent = nullptr);
    struct Title {
        Title& operator=(const size_t newValue) {
            value = newValue;
            return *this;
        }
        bool operator==(const size_t other) {
            return other == value;
        }
        size_t value {};
        bool visible = false;
    };

    struct PressedBlocks {
        size_t m_firstPressedIndex;
        size_t m_secondPressedIndex;
    };

    Q_INVOKABLE bool getItemVisibility(int index);
    Q_INVOKABLE bool showItem(int index);


    int rowCount(const QModelIndex& parent = QModelIndex {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    size_t dimension() const;

signals:
    void checkVisibleStatus();

private:
    void shuffle();
    bool isPositionValid(const size_t position) const;
    void startTimer(uint16_t duration);
    void checkForWin();

    std::vector<Title> m_rawBoard;
    const size_t m_dimension;
    const size_t m_boardSize;
    PressedBlocks pressedBlocks;
    STATE m_state = STATE::NOTHING_PRESSED;
};

#endif // GAMEAREA_H
