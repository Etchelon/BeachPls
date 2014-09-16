#ifndef MACROS_HPP
#define MACROS_HPP

#define QPROPERTY(type, name)				\
	private:										\
		Q_PROPERTY(type name READ name CONSTANT)	\
		type m_##name;								\
	public:											\
		type name() const { return m_##name; }

#define QPROPERTY_WRITE(type, name)					\
	private:												\
		Q_PROPERTY(type name READ name WRITE set_##name)	\
		type m_##name;										\
	public:													\
		type name() const { return m_##name; }				\
	public Q_SLOTS:											\
		void set_##name(type n##name) { m_##name = n##name; }

#define QPROPERTY_NOTIFY(type, name)					\
	private:													\
		Q_PROPERTY(type name READ name NOTIFY name##Changed)	\
		type m_##name;											\
	public:														\
		type name() const { return m_##name; }					\
	private:													\
		void set_##name(type n##name) { if (m_##name != n##name) m_##name = n##name; emit name##Changed(); }	\
	Q_SIGNALS:													\
		void name##Changed();

#define QPROPERTY_WRITE_NOTIFY(type, name)								\
	private:																	\
		Q_PROPERTY(type name READ name WRITE set_##name NOTIFY name##Changed)	\
		type m_##name;															\
	public:																		\
		type name() const { return m_##name; }									\
	public Q_SLOTS:																\
		void set_##name(type n##name) { if (m_##name != n##name) m_##name = n##name; emit name##Changed(); }	\
	Q_SIGNALS:																	\
		void name##Changed();

#endif // MACROS_HPP
