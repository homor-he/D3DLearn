#include "ConvertFunc.h"

DirectX::XMMATRIX ScaleTranslation(DirectX::XMMATRIX matrix, float scale)
{
	matrix.r[3].m128_f32[0] *= scale;
	matrix.r[3].m128_f32[1] *= scale;
	matrix.r[3].m128_f32[2] *= scale;
	return matrix;
}

DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
{
	DirectX::XMMATRIX A = M;
	//逆矩阵的转置只用于计算法线，不希望把位移的转换计算进去
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
}
