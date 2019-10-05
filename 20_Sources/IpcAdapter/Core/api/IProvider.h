#pragma once

#include "Shared/api/ConvenienceMacros.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An abstraction for classes providing concrete realizations of a `T`.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        template<class T>
        class IProvider
        {
        public:
            INTERFACE_DESTRUCTOR(IProvider);

            /**
             * Makes the implementor return the concrete realization of `T`.
             *
             * Ownership stays with the implementor.
             *
             * @retval !=nullptr    implementor provides a realization of `T`
             * @retval ==nullptr    implementor does not provide a realization of `T`
             */
            virtual T* get() const = 0;
        };
    }
}
