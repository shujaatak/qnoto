#include <QDebug>
#include <QAction>
#include <QVBoxLayout>
#include <QSettings>
#include <QMenuBar>
#include "editors.h"
#include "editors-factory.h"
#include "editors-find.h"
#include "history-navigate.h"

#include "includes/file-handler.h"
#include "includes/ui-utils.h"


Editors::Editors():
    m_widget(new QStackedWidget),
    m_find(new EditorsFind)
{
    qnoto::setLayout<QVBoxLayout>(this, m_widget, m_find);

    QAction *a = new QAction(this);
    a->setShortcut(Qt::ControlModifier + Qt::Key_Tab);
    connect(a, &QAction::triggered, this, &Editors::historyNavigate);
    addAction(a);

    qnoto::FileHandler& fh = qnoto::FileHandler::instance();
    connect(&fh, &qnoto::FileHandler::activated, this, &Editors::openFile);
}

Editors::~Editors()
{
}

void Editors::openFile(const QString& file)
{
    static bool block = false;
    if (block)
        return;

    block = true;
    m_stack.removeAll(file);
    m_stack.prepend(file);
    if (m_editors.contains(file)){
        m_widget->setCurrentIndex(m_editors[file].index);
        qnoto::FileHandler::instance().activated(file);
    } else {
        auto* ed = EditorsFactory::create(file);
        connect(ed, &qnoto::EditorInstance::escape, this, &Editors::escape);
        if(ed) {
            int index = m_widget->addWidget(ed);
            m_editors.insert(file, {ed, index});
            m_widget->setCurrentIndex(index);
            qnoto::FileHandler::instance().activated(file);
        }
    }
    m_find->setEditor(m_editors[file].editor);
    m_editors[file].editor->populateMenu(this, m_menuEdit);
    m_editors[file].editor->setFocus();
    block = false;
}

void Editors::showFind()
{
    m_find->activate();
}

void Editors::escape()
{
    if (m_find->isVisible())
        m_find->hide();
}

void Editors::historyNavigate()
{
    if (m_historyNavi) {
        m_historyNavi->next();
    } else {
        m_historyNavi = new HistoryNavigate(m_stack, m_widget);
        connect(m_historyNavi, &HistoryNavigate::closed, this, &Editors::delNavigator);
    }
}

void Editors::delNavigator(const QString& fileName)
{
    openFile(fileName);
    m_historyNavi->deleteLater();
    m_historyNavi = nullptr;
}

void Editors::saveState(QSettings& sets) const
{
    const auto& files = qnoto::FileHandler::instance().openedFiles();

    sets.beginWriteArray("OpenedFiles");
    int count = 0;
    for(const QString& file: files){
        sets.setArrayIndex(count++);
        sets.setValue("file", file);
    }
    sets.endArray();
    qnoto::FileHandler::instance();
}

void Editors::restoreState(QSettings& sets)
{
    int size = sets.beginReadArray("OpenedFiles");
    for(int i = 0; i < size; ++i){
        sets.setArrayIndex(i);
        openFile(sets.value("file").toString());
    }
    sets.endArray();
}

QString Editors::name() const
{
    return "editors";
}

QString Editors::title() const
{
    return tr("Editors");
}

void Editors::setMenu(QMenu* edit)
{
    m_menuEdit = edit;
}

