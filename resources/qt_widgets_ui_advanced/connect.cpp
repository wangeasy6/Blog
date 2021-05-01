static QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection)

SIGNAL

static QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection)

QMetaObject::Connection QObject::connect(const QObject *sender, const char *signal, const char *method, Qt::ConnectionType type = Qt::AutoConnection) const

缺省this

static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *receiver, PointerToMemberFunction method, Qt::ConnectionType type = Qt::AutoConnection)

&

static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, Functor functor)

匿名函数

static QMetaObject::Connection QObject::connect(const QObject *sender, PointerToMemberFunction signal, const QObject *context, Functor functor, Qt::ConnectionType type = Qt::AutoConnection)



static bool QObject::disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)

static bool QObject::disconnect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method)

bool QObject::disconnect(const char *signal = nullptr, const QObject *receiver = nullptr, const char *method = nullptr) const

bool QObject::disconnect(const QObject *receiver, const char *method = nullptr) const

static bool QObject::disconnect(const QMetaObject::Connection &connection)

static bool QObject::disconnect(const QObject *sender, PointerToMemberFunction signal, const QObject *receiver, PointerToMemberFunction method)