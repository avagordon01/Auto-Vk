#pragma once
#include <avk/avk.hpp>

namespace avk
{
	enum struct pipeline_stage : uint32_t
	{
		top_of_pipe = 0x00000001,
		draw_indirect = 0x00000002,
		vertex_input = 0x00000004,
		vertex_shader = 0x00000008,
		tessellation_control_shader = 0x00000010,
		tessellation_evaluation_shader = 0x00000020,
		geometry_shader = 0x00000040,
		fragment_shader = 0x00000080,
		early_fragment_tests = 0x00000100,
		late_fragment_tests = 0x00000200,
		color_attachment_output = 0x00000400,
		compute_shader = 0x00000800,
		transfer = 0x00001000,
		bottom_of_pipe = 0x00002000,
		host = 0x00004000,
		all_graphics = 0x00008000,
		all_commands = 0x00010000,
		transform_feedback = 0x00020000,
		conditional_rendering = 0x00040000,
		command_preprocess = 0x00080000,
		shading_rate_image = 0x00100000,
		ray_tracing_shaders = 0x00200000,
		acceleration_structure_build = 0x00400000,
		task_shader = 0x00800000,
		mesh_shader = 0x01000000,
		fragment_density_process = 0x02000000,
	};

	inline pipeline_stage operator| (pipeline_stage a, pipeline_stage b)
	{
		typedef std::underlying_type<pipeline_stage>::type EnumType;
		return static_cast<pipeline_stage>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline pipeline_stage operator& (pipeline_stage a, pipeline_stage b)
	{
		typedef std::underlying_type<pipeline_stage>::type EnumType;
		return static_cast<pipeline_stage>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}

	inline pipeline_stage& operator |= (pipeline_stage& a, pipeline_stage b)
	{
		return a = a | b;
	}

	inline pipeline_stage& operator &= (pipeline_stage& a, pipeline_stage b)
	{
		return a = a & b;
	}

	inline pipeline_stage exclude(pipeline_stage original, pipeline_stage toExclude)
	{
		typedef std::underlying_type<pipeline_stage>::type EnumType;
		return static_cast<pipeline_stage>(static_cast<EnumType>(original) & (~static_cast<EnumType>(toExclude)));
	}

	inline bool is_included(const pipeline_stage toTest, const pipeline_stage includee)
	{
		return (toTest & includee) == includee;
	}


	class queue;

	struct queue_ownership
	{
		queue_ownership& from(queue* aSrcQueue) { mSrcQueue = aSrcQueue; return *this; }
		queue_ownership& to(queue* aDstQueue) { mDstQueue = aDstQueue; return *this; }

		queue_ownership* operator-> () { return this; }

		std::optional<queue*> mSrcQueue;
		std::optional<queue*> mDstQueue;
	};

	class buffer_t;

	struct pipeline_barrier_buffer_data
	{
		pipeline_barrier_buffer_data(buffer_t& aBufferRef)
			: mBufferRef(&aBufferRef)
		{
		}

		pipeline_barrier_buffer_data* operator-> () { return this; }

		pipeline_barrier_buffer_data& operator>> (const queue_ownership& aQueueOwnershipTransfer)
		{
			mSrcQueue = aQueueOwnershipTransfer.mSrcQueue;
			mDstQueue = aQueueOwnershipTransfer.mDstQueue;
			return *this;
		}

		buffer_t* mBufferRef; // TODO: should be owning resource
		std::optional<queue*> mSrcQueue;
		std::optional<queue*> mDstQueue;
	};

	using for_buffer = pipeline_barrier_buffer_data;

	class command_buffer_t;

	namespace stage
	{
		struct pipeline_stage2
		{
			vk::PipelineStageFlags2KHR mSrc;
			vk::PipelineStageFlags2KHR mDst;
		};

		struct pipeline_stage_flags
		{
			vk::PipelineStageFlags2KHR mFlags;
		};

		inline pipeline_stage2 operator>> (pipeline_stage_flags a, pipeline_stage_flags b)
		{
			return pipeline_stage2{ a.mFlags, b.mFlags };
		}

#pragma region pipeline_stage_flags operators
		inline pipeline_stage_flags operator| (pipeline_stage_flags a, pipeline_stage_flags b)
		{
			return pipeline_stage_flags{ a.mFlags | b.mFlags };
		}

		inline pipeline_stage_flags operator& (pipeline_stage_flags a, pipeline_stage_flags b)
		{
			return pipeline_stage_flags{ a.mFlags & b.mFlags };
		}

		inline pipeline_stage_flags& operator |= (pipeline_stage_flags& a, pipeline_stage_flags b)
		{
			return a = a | b;
		}

		inline pipeline_stage_flags& operator &= (pipeline_stage_flags& a, pipeline_stage_flags b)
		{
			return a = a & b;
		}

		inline pipeline_stage_flags exclude(pipeline_stage_flags original, pipeline_stage_flags toExclude)
		{
			return pipeline_stage_flags{ original.mFlags & ~toExclude.mFlags };
		}

		inline bool is_included(const pipeline_stage_flags toTest, const pipeline_stage_flags includee)
		{
			return (toTest.mFlags & includee.mFlags) == includee.mFlags;
		}
#pragma endregion

#pragma region pipeline_stage2 | pipeline_stage_flags operators
		inline pipeline_stage2 operator| (pipeline_stage2 a, pipeline_stage_flags b)
		{
			return pipeline_stage2{ a.mSrc, a.mDst | b.mFlags };
		}

		inline pipeline_stage2 operator& (pipeline_stage2 a, pipeline_stage_flags b)
		{
			return pipeline_stage2{ a.mSrc, a.mDst & b.mFlags };
		}

		inline pipeline_stage2& operator |= (pipeline_stage2& a, pipeline_stage_flags b)
		{
			return a = a | b;
		}

		inline pipeline_stage2& operator &= (pipeline_stage2& a, pipeline_stage_flags b)
		{
			return a = a & b;
		}
#pragma endregion

#pragma region pipeline_stage_flags | pipeline_stage2 operators
		inline pipeline_stage2 operator| (pipeline_stage_flags a, pipeline_stage2 b)
		{
			return pipeline_stage2{ a.mFlags | b.mSrc, b.mDst };
		}

		inline pipeline_stage2 operator& (pipeline_stage_flags a, pipeline_stage2 b)
		{
			return pipeline_stage2{ a.mFlags & b.mSrc, b.mDst };
		}
#pragma endregion

		static const auto none                             = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eNone };
		static const auto top_of_pipe                      = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTopOfPipe };
		static const auto draw_indirect                    = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eDrawIndirect };
		static const auto vertex_input                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eVertexInput };
		static const auto vertex_shader                    = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eVertexShader };
		static const auto tessellation_control_shader      = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTessellationControlShader };
		static const auto tessellation_evaluation_shader   = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTessellationEvaluationShader };
		static const auto geometry_shader                  = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eGeometryShader };
		static const auto fragment_shader                  = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eFragmentShader };
		static const auto early_fragment_tests             = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eEarlyFragmentTests };
		static const auto late_fragment_tests              = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eLateFragmentTests };
		static const auto color_attachment_output          = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eColorAttachmentOutput };
		static const auto compute_shader                   = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eComputeShader };
		static const auto all_transfer                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eAllTransfer };
		static const auto transfer                         = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTransfer };
		static const auto bottom_of_pipe                   = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eBottomOfPipe };
		static const auto host                             = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eHost };
		static const auto all_graphics                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eAllGraphics };
		static const auto all_commands                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eAllCommands };
		static const auto copy                             = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eCopy };
		static const auto resolve                          = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eResolve };
		static const auto blit                             = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eBlit };
		static const auto clear                            = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eClear };
		static const auto index_input                      = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eIndexInput };
		static const auto vertex_attribute_input           = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eVertexAttributeInput };
		static const auto pre_rasterization_shaders        = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::ePreRasterizationShaders };
		static const auto video_decode                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eVideoDecode };
		static const auto video_encode                     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eVideoEncode };
		static const auto transform_feedback               = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTransformFeedbackEXT };
		static const auto conditional_rendering            = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eConditionalRenderingEXT };
		static const auto command_preprocess               = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eCommandPreprocessNV };
		static const auto fragment_shading_rate_attachment = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eFragmentShadingRateAttachment };
		static const auto shading_rate_image               = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eShadingRateImageNV };
		static const auto acceleration_structure_build     = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eAccelerationStructureBuild };
		static const auto ray_tracing_shader               = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eRayTracingShader };
		static const auto fragment_density_process         = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eFragmentDensityProcessEXT };
		static const auto task_shader                      = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eTaskShaderNV };
		static const auto mesh_shader                      = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eMeshShaderNV };
		static const auto subpass_shading                  = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eSubpassShadingHUAWEI };
		static const auto invocation_mask                  = pipeline_stage_flags{ vk::PipelineStageFlagBits2KHR::eInvocationMaskHUAWEI };
	}
}
