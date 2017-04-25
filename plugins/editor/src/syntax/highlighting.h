#pragma once
#include <QSyntaxHighlighter>
#include "definition.h"

class QTextDocument;

namespace syntax {

FORWARD(Theme)
struct State;

class Highlighting: public QSyntaxHighlighter
{
public:
    Highlighting(QTextDocument* doc, const DefinitionPtr& def, bool markWhite = true);
    void setTheme(const ThemePtr& theme);

    void setFound(QTextBlock& block, const std::map<int, QString>& idx);
    void clearFound(QTextBlock& block);
    bool paintBlock(const QTextBlock& block, QPainter& painter, const QRect& bounding);
protected:
    void highlightBlock(const QString &text) override;
    void highlightLine(const QString& text, const QSharedPointer<State>& state);
    bool fetchFormat(const QString& frm, QTextCharFormat& format);
    void applyFormat(const QString& frm, int from, int to);
    void applyFormat(const QTextCharFormat& format, int from, int length);
    void contextChanged(const ContextPtr& oc, int& begin, int end);
private:
    DefinitionPtr m_definition;
    ThemePtr      m_theme;
    bool          m_markWhite;
};

}
