#ifndef SETTING_H
#define SETTING_H

#include "Model.h"
#include <QString>

class Setting : public Model
{
  public:
    Setting() = default;

    QString getKey() const
    {
        return key;
    }

    QString getValue() const
    {
        return value;
    }

    void setKey(const QString& key)
    {
        this->key = key;
    }

    void setValue(const QString& value)
    {
        this->value = value;
    }

  private:
    QString key;
    QString value;
};

#endif // SETTING_H
