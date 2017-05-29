#include "Notes.h"

Note::Note(const QString & i, const QString & ti) : id(i), title(ti)
{
}

void Note::setTitle(const QString & t)
{
	title = t;
}

void Note::saveInFile(QString& folder) const
{
	QString file(folder +"/"+id + ".xml");

	QFile newfile(file);
	if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		//a faire, mettre NoteException dans une classe s�par�e
		//throw NotesException(QString("erreur sauvegarde notes : ouverture fichier xml"));
	}
	QXmlStreamWriter stream(&newfile);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("notes");
    stream.writeStartElement(getType());
	stream.writeTextElement("id", id);
	stream.writeTextElement("title", title);

	saveAttributesInFile(stream);

	stream.writeEndElement();
	stream.writeEndElement();
	stream.writeEndDocument();
	newfile.close();

    qDebug()<<"saved note"<<id<<"in file";

}

Note::~Note() {}


