#include "NoteEditeur.h"

NoteEditeur::NoteEditeur(Note& a, QWidget *parent) : QWidget(parent), note(&a)
{
	idl = new QLabel("Identificateur");
	titrel = new QLabel("Titre");
	save = new QPushButton("Sauver");

    idl->setMinimumWidth(75);
    titrel->setMinimumWidth(75);
    id = new QLineEdit();
    titre = new QLineEdit();

    mainLayout = new QVBoxLayout();
    idLayout = new QHBoxLayout();
    titreLayout = new QHBoxLayout();
    centralLayout = new QHBoxLayout();

    idLayout->addWidget(idl);
    titreLayout->addWidget(titrel);

    idLayout->addWidget(id);
    titreLayout->addWidget(titre);

    mainLayout->addLayout(idLayout);
    mainLayout->addLayout(titreLayout);
    mainLayout->addLayout(centralLayout);
    mainLayout->addWidget(save);

    this->setLayout(mainLayout);

	id->setReadOnly(true);
	id->setText(a.getId());
	titre->setText(a.getTitle());
	save->setEnabled(false);

	QObject::connect(titre, SIGNAL(textChanged(QString)), this, SLOT(activerSave()));
	QObject::connect(save, SIGNAL(clicked()), this, SLOT(sauvegarde()));

}


NoteEditeur::~NoteEditeur()
{

}
void NoteEditeur::sauvegarde()
{
	note->setTitle(titre->text());
	sauvegardeAttributs();

	//note->saveInFile(); imposssible car foldername dans NoteManager solutions : stocker le foldername autre part, ou le chemin entier de la note dans chaque note ?
	
	save->setDisabled(true);
}
void NoteEditeur::verifSave()  {

	if (save->isEnabled()) {
		NoteEditeurFermeture* x = new NoteEditeurFermeture();
		int dialogCode = x->exec();
		if (dialogCode == QDialog::Accepted) {
			sauvegarde();
		}
	}

}
void NoteEditeur::activerSave() {
	save->setEnabled(true);
}
