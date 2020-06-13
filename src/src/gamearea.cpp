#include "gamearea.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <random>
#include <QDebug>

#include <iostream>

GameArea::GameArea(size_t dimension, QObject* parent)
    : QAbstractListModel {parent}
    , m_dimension {dimension}
    , m_boardSize {dimension * dimension}

{
    m_rawBoard.resize(m_boardSize);
    std::iota(m_rawBoard.begin(), m_rawBoard.begin() + rowCount() / 2, 1);
    std::iota(m_rawBoard.begin() + rowCount() / 2, m_rawBoard.end(), 1);

    shuffle();
}

void GameArea::shuffle()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);
    std::shuffle(m_rawBoard.begin(), m_rawBoard.end(), generator);
}

int GameArea::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_rawBoard.size());
}

QVariant GameArea::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }
    const auto rowIndex {static_cast<size_t>(index.row())};
    if (!isPositionValid(rowIndex)) {
        return {};
    }

    return QVariant(static_cast<int>(m_rawBoard[rowIndex].value));
}

bool GameArea::isPositionValid(const size_t position) const
{
    return (position < m_boardSize);
}

size_t GameArea::dimension() const
{
    return m_dimension;
}

bool GameArea::showItem(int index)
{
    if(!isPositionValid(static_cast<size_t>(index))) {
        return false;
    }

    if (m_rawBoard[static_cast<size_t>(index)].visible) {
        return false;
    }

    switch (m_state) {
        case STATE::NOTHING_PRESSED:
            m_state = STATE::FIRST_PRESSED;
            pressedBlocks.m_firstPressedIndex = static_cast<size_t>(index);
            m_rawBoard[static_cast<size_t>(pressedBlocks.m_firstPressedIndex)].visible = true;
            break;
        case STATE::FIRST_PRESSED:
            m_state = STATE::SECOND_PRESSED;
            pressedBlocks.m_secondPressedIndex = static_cast<size_t>(index);
            m_rawBoard[pressedBlocks.m_secondPressedIndex].visible = true;
            if (m_rawBoard[pressedBlocks.m_firstPressedIndex].value == m_rawBoard[pressedBlocks.m_secondPressedIndex].value) {
                m_state = STATE::NOTHING_PRESSED;
                checkForWin();
            } else {
                std::thread t1(&GameArea::startTimer, this, 1);
                t1.detach();
            }
            break;
        case STATE::SECOND_PRESSED:
            qDebug () << "IGNORE ALL PRESSED ";
            break;
    }

    return true;
}

void GameArea::checkForWin()
{
    const auto it = std::find_if(m_rawBoard.begin(), m_rawBoard.end(), [](Title t){ return t.visible == false; });
    if (it == m_rawBoard.end()) {
        qDebug () << "You WIN"; // ToDo: add some logic for win
    }
}

void GameArea::startTimer(uint16_t duration)
{
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    m_state = STATE::NOTHING_PRESSED;
    m_rawBoard[pressedBlocks.m_firstPressedIndex].visible = false;
    m_rawBoard[pressedBlocks.m_secondPressedIndex].visible = false;
    emit checkVisibleStatus();
}

bool GameArea::getItemVisibility(int index)
{
    return m_rawBoard[static_cast<size_t>(index)].visible;
}

