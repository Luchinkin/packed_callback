# Packed callback

## Requires C++ 20

## Example:
```cpp
template<typename... _args_type>
static void for_each_cpu_ipi( auto&& callback, _args_type&&... arguments ) noexcept
{
	auto packed_callback_data = pack_callback_data( callback, arguments... );

	KeIpiGenericCall( packed_callback_data.get_callback<PKIPI_BROADCAST_WORKER>(),
					  packed_callback_data.get_context<uint64_t*>() );
}
```
