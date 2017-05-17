#include "ArticleEditeur.h"



ArticleEditeur::ArticleEditeur(Article& a, QWidget *parent) : NoteEditeur(a,parent)
{
	textl = new QLabel("Text");

	text = new QTextEdit;

    centralLayout->addWidget(textl);

    centralLayout->addWidget(text);

	text->setText(a.getText());
	
	QObject::connect(text, SIGNAL(textChanged()), this, SLOT(activerSave()));

}


ArticleEditeur::~ArticleEditeur()
{

}
void ArticleEditeur::sauvegardeAttributs()
{	
	dynamic_cast<Article&>(getNote()).setText(text->toPlainText());
}


