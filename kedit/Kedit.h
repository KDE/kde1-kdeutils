#ifndef kedit_h
#define kedit_h

#include <qlist.h>
#include <qstring.h>
#include "kedit.h"
/*
 * kedit.h
 * defines toplevel widget for kedit
 *
 * $Id$
 */

class TopLevel : public QWidget
{
    Q_OBJECT;
public:
    /// Tells us what kind of job kedit is waiting for.
    enum action { GET, PUT };
    
    TopLevel( QWidget *parent=0, const char *name=0 );
    ~TopLevel();
    KEdit *eframe;

    /// Works like the load method of the edit widget but is able to talk to KFM
    void openNetFile( const char *_url, int _mode );

    /// Saves the current text to the URL '_url'.
    void saveNetFile( const char *_url );

    /// List of all windows
    static QList<TopLevel> windowList;

protected:
    void resizeEvent(QResizeEvent *);
    void setSensitivity();
        
private:    
    QMenuBar *menubar;
    QPopupMenu *file, *edit, *help;
    int open_mode;

    /// KFM client
    /**
      Only one KFM connection should be opened at once. Otherwise kedit could get
      confused. If this is 0L, you may create a new connection to kfm.
      */
    KFM * kfm;
    
    /// Temporary file for internet purposes
    /**
      If KEdit is waiting for some internet task to finish, this is the
      file that is involved. Mention that it is a complete URL like
      "file:/tmp/mist.txt".
      */
    QString tmpFile;

    /// If we load a file from the net this is the corresponding URL
    QString netFile;
    
    /// Tells us what kfm is right now doing for us
    /**
      If this is for example GET, then KFM loads a file from the net
      to the local file system.
      */
    action kfmAction;
    
    /// The open mode for a net file
    /**
      If KEdit is waiting for an internet file, this is the mode in which
      it should open the file.
      */
    int openMode;

    //QString helpurl;
    
public slots:
    void menuCallback(int);
    void setFileCaption();

    /// Gets signals from KFM
    void slotKFMFinished();

    /// Drag and Drop
    void slotDropEvent( KDNDDropZone * _dropZone );
};

#endif
