#pragma once
#include "rule.h"

namespace syntax {

FORWARD(HlCStringChar)

class HlCStringChar: public Rule
{
public:
    HlCStringChar();
    int match(const QString& text, int offset) override;
};

}