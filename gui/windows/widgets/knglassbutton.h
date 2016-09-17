/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNGLASSBUTTON_H
#define KNGLASSBUTTON_H

#include <QBitmap>

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNGlassButton class provides a glass looked button. But It seems
 * that this button will cost a large memeory used. Don't use this button a lot.
 * \n
 * It's designed to copy the Aero Glass from the Window Media Center of Windows
 * 7.
 */
class Q_DECL_EXPORT KNGlassButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNGlassButton widget.
     * \param parent The given parent widget.
     */
    explicit KNGlassButton(QWidget *parent = 0);

    /*!
     * \brief Get the spacing between the icon and the text.
     * \return The spacing size.
     */
    int spacing() const;

    /*!
     * \brief Set the spacing size of the icon and the text.
     * \param spacing The spacing size.
     */
    void setSpacing(int spacing);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    void startAnime(const int &endFrame);
    QPixmap m_maskImage;
    QLinearGradient m_highLight, m_highLightMask;
    qreal m_halfWidth, m_indicatorX;
    Qt::Alignment m_contentAlign;
    int m_spacing, m_imageOpacity;
    QTimeLine *m_mouseAnime;
};

#endif // KNGLASSBUTTON_H
