#pragma once
#include <QPlainTextEdit>
#include <QScopedPointer>
#include "includes/plugin.h"

namespace syntax
{
FORWARD(Definition)
FORWARD(Highlighting)
FORWARD(Theme)
}
class EditorMargin;

class Finder
{
public:
    QString pattern;
    bool isSet = false;
public:
};


class EditorImpl: public QPlainTextEdit
{
    Q_OBJECT
friend class EditorMargin;
public:
    EditorImpl(const QString& fname, const syntax::DefinitionPtr& def);
    ~EditorImpl();

    void init();
    const syntax::ThemePtr& theme() const;
    void load(const QString& text);
    const QString& fileName() const;
public slots:
    void startFind(const QString& text);
    void unmark();
protected:
    void resizeEvent(QResizeEvent*e ) override;
    void paintEvent(QPaintEvent* e) override;
private:
    QString                              m_fileName;
    QScopedPointer<syntax::Highlighting> m_syntax;
    EditorMargin*                        m_margin = nullptr;
    syntax::ThemePtr                     m_theme;
    Finder                               m_finder;
};