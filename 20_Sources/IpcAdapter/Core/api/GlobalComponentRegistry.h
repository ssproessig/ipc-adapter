#pragma once

#include "Core/api/IFactoryRegistry.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * The global component registry following the Singleton pattern.
         *
         * Simplifies registering components from their implementation.
         *
         * NOTE: does only work as long as we have static libraries! With dynamically libraries this must be changed
         *       to a registration API per library.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class GlobalComponentRegistry
        {
            STATIC_CLASS(GlobalComponentRegistry);

        public:
            /**
             * Returns the one-and-only factory registry used in the Core.
             *
             * @return one-and-only factory registry in the Core.
             */
            static IFactoryRegistry& get();

        };
    }
}





/**
 * A helper macro to avoid "god registry" that needs to include all component headers. Instead,
 * only include GlobalComponentRegistry.h and use this macro to publish your component.
 *
 * @param COMPONENT_ID  id of the component to register
 */
#define REGISTER_COMPONENT(COMPONENT_ID)															\
namespace                                                                                           \
{                                                                                                   \
    struct RegisterComponent                                                                        \
    {                                                                                               \
        RegisterComponent()                                                                         \
        {                                                                                           \
            IpcAdapter::Core::GlobalComponentRegistry::get().registerFactoryFor(                    \
                #COMPONENT_ID,																		\
				[]()->std::shared_ptr<IpcAdapter::Core::IComponent> {								\
					return std::make_shared<COMPONENT_ID>();										\
				});																					\
        }                                                                                           \
    } registerIt;																					\
																									\
}
