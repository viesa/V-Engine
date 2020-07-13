#include "TransformCBuf.h"

TransformCBuf::TransformCBuf( Graphics &gfx, const Drawable &parent )
	:
	m_vCBuf( gfx ),
	m_parent( parent )
{
}

void TransformCBuf::BindTo( Graphics &gfx ) noexcept
{
	m_vCBuf.Update(
		gfx,
		DirectX::XMMatrixTranspose(
			m_parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	m_vCBuf.BindTo( gfx );
}