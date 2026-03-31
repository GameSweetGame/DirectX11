#pragma once

#include <vector>
#include <map>
#include <string>

#include <SimpleMath.h>

struct KeyAnimation
{
	double Time;
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Quaternion Rotation;
	DirectX::SimpleMath::Vector3 Scaling;
};

struct AnimationNode
{
	std::vector<KeyAnimation> KeyAnimations;

	DirectX::SimpleMath::Matrix Interpolation(double progressTime) const
	{
		using namespace DirectX::SimpleMath;

		Vector3 position;
		Quaternion rotation;
		Vector3 scaling;

		for (auto iter = KeyAnimations.begin(); iter != KeyAnimations.end(); ++iter)
		{
			if (iter->Time > progressTime)
			{
				auto begin = iter - 1;
				float delta = progressTime - begin->Time;
				float deltaRatio = delta / (iter->Time - begin->Time);

				position = Vector3::Lerp(begin->Position, iter->Position, deltaRatio);
				rotation = Quaternion::Slerp(begin->Rotation, iter->Rotation, deltaRatio);
				scaling = Vector3::Lerp(begin->Scaling, iter->Scaling, deltaRatio);

				break;
			}
		}

		return Matrix::CreateScale(scaling) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position);
	}
};

struct Animation
{
	double Duration;
	std::map<std::string, AnimationNode> AnimationNodes;
};