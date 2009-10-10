#include "accountlist.h"

AccountList::AccountList( QWidget* parent )
    : QTreeWidget( parent )
{
    this->setHeaderHidden( true );
    this->setColumnCount( 1 );
}
