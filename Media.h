#ifndef MEDIA_H
#define MEDIA_H
#include "Notes.h"


class Media : public Note {

private:
    friend class NotesManager;

    QString description;
    QString file;
    void saveAttributesInFile(QXmlStreamWriter& stream) const;

protected :
    Media(const QString& fp, const QString& ti="", const QString& des="", const QString& f="");

public:
    QString getDescription() const { return description; }
    QString getFile() const { return file; }
    QString getType() const;
    void setDescription(const QString& desc);
    void setFile(const QString& f);

};

#endif // MEDIA_H


