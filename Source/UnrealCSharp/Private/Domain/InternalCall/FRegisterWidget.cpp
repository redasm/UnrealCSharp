#include "Binding/Class/TBindingClassBuilder.inl"
#include "Binding/Property/TPropertyBuilder.inl"
#include "CoreMacro/NamespaceMacro.h"
#include "Macro/BindingMacro.h"
#include "Common/FUnrealCSharpFunctionLibrary.h"
#include "Components/Widget.h"

namespace
{
	struct FRegisterWidget
	{
		static bool IsDesignTimeImplementation(const FGarbageCollectionHandle InGarbageCollectionHandle)
		{
			if (const auto FoundWidget = FCSharpEnvironment::GetEnvironment().GetObject<UWidget>(InGarbageCollectionHandle))
			{
				return FoundWidget->IsDesignTime();
			}

			return false;
		}

		FRegisterWidget()
		{
			TBindingClassBuilder<UWidget>(NAMESPACE_LIBRARY)
				.Function("IsDesignTime", IsDesignTimeImplementation);
		}
	};

	[[maybe_unused]] FRegisterWidget RegisterClass;
}
