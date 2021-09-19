#pragma once
#include <utility>
#include <tuple>

template<typename... _args_type>
struct packed_callback_data_t
{
public:
	struct packed_context_t
	{
		std::tuple<_args_type...> packed_args;
		void* callback_ptr;
	};

	void ( *packed_callback )( void* );
	packed_context_t packed_context;

	template <typename _callback_type>
	_callback_type get_callback() noexcept { return reinterpret_cast<_callback_type>( this->packed_callback ); }

	template <typename _context_type>
	_context_type get_context() noexcept { return reinterpret_cast<_context_type>( &this->packed_context ); }
};

template<typename... _args_type>
packed_callback_data_t<_args_type...> pack_callback_data( auto&& callback, _args_type&&... arguments ) noexcept
{
	return packed_callback_data_t<_args_type...>{
		.packed_callback = []( void* context_ptr )
		{
			auto internal_context_ptr = reinterpret_cast<packed_callback_data_t<_args_type...>::packed_context_t*>( context_ptr );
			std::apply( reinterpret_cast<decltype( callback )>( internal_context_ptr->callback_ptr ), internal_context_ptr->packed_args );
		},
		.packed_context =
		{
			.packed_args = std::forward_as_tuple( arguments... ),
			.callback_ptr = &callback
		}
	};
}

