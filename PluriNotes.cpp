#include "PluriNotes.h"
#include "NoteEditeur.h"
#include "nouvellenote.h"
#include <qDebug>
#include <QList>

PluriNotes::PluriNotes(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    m_sSettingsFile = QDir::currentPath() + "/config.ini";


    QFileInfo check_file(m_sSettingsFile);

    if (!check_file.exists()){
        QSettings* settings = new QSettings(m_sSettingsFile, QSettings::IniFormat);
        settings->setValue("folder", QDir::currentPath());
        settings->sync();
        qDebug()<<"file created"<<endl;
    }

    loadSettings();
    ouvrirProjet();
	QObject::connect(ui.actionNote, SIGNAL(triggered()), this, SLOT(nouvelleNote()));
    QObject::connect(ui.actionOptions, SIGNAL(triggered()), this, SLOT(openSettings()));
}


void PluriNotes::loadSettings()
{

    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    QString sText = settings.value("folder", "").toString();

    NotesManager& m = NotesManager::getManager();
    m.setFoldername(sText);

    qDebug()<<"settings loaded"<<settings.value("folder", "").toString()<<endl;

}

void PluriNotes::closeEvent(QCloseEvent *event)
{
    NoteEditeur* noteEdit = dynamic_cast<NoteEditeur*>(ui.horizontalLayout->itemAt(1)->widget());
    if (noteEdit) {
        noteEdit->verifSave();
	}
}

void PluriNotes::openSettings()
{
    SettingsDialog* x = new SettingsDialog(m_sSettingsFile);
    if(x->exec() == QDialog::Accepted){

        //si on a sauvegardé des changements dans les paramètres, on recharge le projet



        QWidget* old = ui.horizontalLayout->itemAt(1)->widget();
        NoteEditeur* noteEdit = dynamic_cast<NoteEditeur*>(old);
        if (noteEdit){
            noteEdit->verifSave();
            ui.horizontalLayout->replaceWidget(old,new QWidget());
            delete old;
        }
        NotesManager::freeManager();
        ui.listWidget->clear();
        loadSettings();
        ouvrirProjet();
    }
}

void PluriNotes::ouvrirProjet() {

    NotesManager& m = NotesManager::getManager();

    qDebug()<<"folder:"<<m.getFoldername();

	m.load();
	for (NotesManager::Iterator it = m.getIterator(); !it.isDone(); it.next()) {
			new QListWidgetItem(it.current().getId(), ui.listWidget);
	}

	QObject::connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(ouvrirNote(QListWidgetItem*)));
	//On active le bouton nouvelle note
	ui.actionNote->setEnabled(true);
}

void PluriNotes::ouvrirNote(QListWidgetItem* item) {

	QWidget* old = ui.horizontalLayout->itemAt(1)->widget();
	NoteEditeur* noteEdit = dynamic_cast<NoteEditeur*>(old);

	//On lance la fenetre de verification de fermeture si on ferme un article, si le bouton de sauvegarde est activé, et si la nouvelle note est différente de l'ancienne.
	if (noteEdit && noteEdit->getId()!=item->text()) {
		noteEdit->verifSave();
	}
	//si on deja sur la note, on ne fait rien
	else if (noteEdit && noteEdit->getId() == item->text()) {
		return;
	}

	NotesManager& m = NotesManager::getManager();
	Note& n = m.getNote(item->text());

	NoteEditeur* fenetre = m.createEditor(&n);

	ui.horizontalLayout->replaceWidget(old,fenetre);
	delete old;
	//**********Code pour Mdi window

	//QMdiSubWindow* sousFenetre = new QMdiSubWindow;
	//sousFenetre->setWidget(fenetre);
	//
	//QList<QMdiSubWindow*> fenetres = ui.mdiArea->subWindowList();
}

void PluriNotes::nouvelleNote()
{
	NouvelleNote* x = new NouvelleNote();
	if (x->exec() == QDialog::Accepted) {

		// Ajouter : tri par ordre alphabetique de la liste, verif si la note n'existe pas deja
		QListWidgetItem* nouvelle_note = new QListWidgetItem(x->getNom(), ui.listWidget);
		NotesManager& m = NotesManager::getManager();

		m.create(x->getSelectedType(),x->getNom());
        m.saveAllNotes();
		ouvrirNote(nouvelle_note);

	}
	delete x;
}

PluriNotes::~PluriNotes()
{
    NotesManager::freeManager();
}
