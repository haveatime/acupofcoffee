#ifndef EXTERNAL__LOCK__TEMPLATE__H__
#define EXTERNAL__LOCK__TEMPLATE__H__

#ifdef USE_BOOST_EXTERNAL_LOCK
#define BOOST_ALL_DYN_LINK
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/externally_locked.hpp>

#define abcplat boost
#else

#ifdef USE_QT_QMUTEX
#include <QtCore/QMutex>
namespace abcplat {

	class recursive_mutex : public QMutex
	{
	public:
		recursive_mutex()
			:QMutex(QMutex::Recursive)
		{
		}
		~recursive_mutex()
		{
		}
	};
}
#else
#define BOOST_ALL_DYN_LINK
#include <boost/thread/recursive_mutex.hpp>

namespace abcplat {

	class recursive_mutex : public boost::detail::basic_recursive_mutex_impl<boost::detail::basic_timed_mutex>
	{
	public:
		recursive_mutex()
		{
			boost::detail::basic_recursive_mutex_impl<boost::detail::basic_timed_mutex>::initialize();
		}
		~recursive_mutex()
		{
			boost::detail::basic_recursive_mutex_impl<boost::detail::basic_timed_mutex>::destroy();
		}
	};
}
#endif

namespace abcplat {

	template <typename BasicLockable>
	class basic_lockable_adapter
	{
	public:
		typedef BasicLockable mutex_type;

	protected:
		mutex_type& lockable() const
		{
			return lockable_;
		}
		mutable mutex_type lockable_;
	public:

		basic_lockable_adapter()
		{
		}

		void lock() const
		{
			lockable().lock();
		}

		void unlock() const
		{
			lockable().unlock();
		}
	};

	template <typename Lockable>
	class strict_lock
	{
	public:
		typedef Lockable mutex_type;

		explicit strict_lock(mutex_type& mtx)
			:mtx_(mtx)
		{
			mtx.lock();
		}

		~strict_lock()
		{
			mtx_.unlock();
		}

		mutex_type* mutex()
		{
			return &mtx_;
		}

		bool owns_lock()
		{
			return true;
		}

		bool owns_lock(const mutex_type* l)
		{
			return l == mutex();
		}

	private:
		mutex_type& mtx_;
	};

	template <typename T, typename MutexType>
	class externally_locked
	{
	public:
		typedef MutexType mutex_type;

		externally_locked(mutex_type& mtx)
			: obj_(), mtx_(&mtx)
		{
		}

		T& get(strict_lock<mutex_type>& lk)
		{
			if (!lk.owns_lock(mtx_))
				throw "mutex bad match";

			return obj_;
		}

		void lock()
		{
			mtx_->lock();
		}

		void unlock()
		{
			mtx_->unlock();
		}

	private:
		T obj_;
		mutex_type* mtx_;
	};
}
#endif

#endif
