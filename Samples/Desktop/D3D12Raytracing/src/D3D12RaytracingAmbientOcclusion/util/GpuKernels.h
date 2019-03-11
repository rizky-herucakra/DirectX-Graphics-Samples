//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//
// Helpers for doing CPU & GPU performance timing and statitics
//

#pragma once

namespace GpuKernels
{
	class ReduceSum
	{
	public:
        enum Type {
            Uint = 0,
            Float
        };

		void Release()
		{
			assert(0 && L"ToDo");
		}

		void Initialize(ID3D12Device* device, Type type);
		void CreateInputResourceSizeDependentResources(
			ID3D12Device* device,
			DX::DescriptorHeap* descriptorHeap,
			UINT frameCount,
			UINT width,
			UINT height,
			UINT numInvocationsPerFrame);
		void Execute(
			ID3D12GraphicsCommandList* commandList,
			ID3D12DescriptorHeap* descriptorHeap,
			UINT frameIndex,
			UINT invocationIndex,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
			void* resultSum);

	private:
        Type                                m_resultType;
        UINT                                m_resultSize;
		ComPtr<ID3D12RootSignature>         m_rootSignature;
		ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
		std::vector<RWGpuResource>			m_csReduceSumOutputs;
		std::vector<ComPtr<ID3D12Resource>>	m_readbackResources;
	};

	class DownsampleBoxFilter2x2
	{
	public:
		void Release()
		{
			assert(0 && L"ToDo");
		}

		void Initialize(ID3D12Device* device);
		void CreateInputResourceSizeDependentResources(
			ID3D12Device* device,
			UINT width,
			UINT height);
		void Execute(
			ID3D12GraphicsCommandList* commandList,
			UINT width,
			UINT height,
			ID3D12DescriptorHeap* descriptorHeap,
			const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
			const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle);

	private:
		ComPtr<ID3D12RootSignature>         m_rootSignature;
		ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
		std::vector<RWGpuResource>			m_csReduceSumOutputs;
		std::vector<ComPtr<ID3D12Resource>>	m_readbackResources;
		ConstantBuffer<DownsampleFilterConstantBuffer> m_CB;
	};


	class DownsampleGaussianFilter
	{
	public:
        // ToDo images moves switching between.
		enum Type {
			Tap9 = 0,
			Tap25
		};

		void Release()
		{
			assert(0 && L"ToDo");
		}

		void Initialize(ID3D12Device* device, Type type);
		void CreateInputResourceSizeDependentResources(
			ID3D12Device* device,
			UINT width,
			UINT height);
		void Execute(
			ID3D12GraphicsCommandList* commandList,
			UINT width,
			UINT height,
			ID3D12DescriptorHeap* descriptorHeap,
			const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
			const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle);

	private:
		ComPtr<ID3D12RootSignature>         m_rootSignature;
		ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
		ConstantBuffer<DownsampleFilterConstantBuffer> m_CB;
	};

    // ToDo rename to GBuffer downsample
    class DownsampleBilateralFilter
    {
    public:
        enum Type {
            Filter2X2 = 0,
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device, Type type);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            UINT width,
            UINT height,
            ID3D12DescriptorHeap* descriptorHeap,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputPositionResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputGeometryHitResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputNormalResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputPositionResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputGeometryHitResourceHandle);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
    };

    class UpsampleBilateralFilter
    {
    public:
        enum Type {
            Filter2X2 = 0,
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device, Type type);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            UINT width,
            UINT height,
            ID3D12DescriptorHeap* descriptorHeap,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputLowResNormalResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputHiResNormalResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
    };

    class GaussianFilter
    {
    public:
        enum FilterType {
            Filter3X3 = 0,
            Count
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            UINT width,
            UINT height,
            FilterType type,
            ID3D12DescriptorHeap* descriptorHeap,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObjects[FilterType::Count];
        ConstantBuffer<GaussianFilterConstantBuffer> m_CB;
    };


    class RootMeanSquareError
    {
    public:
        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device);
        void CreateInputResourceSizeDependentResources(
            ID3D12Device* device,
            DX::DescriptorHeap* descriptorHeap,
            UINT frameCount,
            UINT width,
            UINT height,
            UINT numInvocationsPerFrame);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            ID3D12DescriptorHeap* descriptorHeap,
            UINT frameIndex,
            UINT invocationIndex,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle,
            float* rootMeanSquareError);

    private:
        typedef UINT ResultType;
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObject;
        RWGpuResource			            m_perPixelMeanSquareError;
        ReduceSum                           m_reduceSumKernel;
    };

    class AtrousWaveletTransformCrossBilateralFilter
    {
    public:
        enum Mode {
            OutputFilteredValue,
            OutputPerPixelFilterWeightSum
        };

        enum FilterType {
            EdgeStoppingBox3x3 = 0,
            EdgeStoppingGaussian3x3,
            EdgeStoppingGaussian5x5,
            Count
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device, UINT maxFilterPasses);
        void CreateInputResourceSizeDependentResources(
            ID3D12Device* device,
            DX::DescriptorHeap* descriptorHeap,
            UINT width,
            UINT height);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            ID3D12DescriptorHeap* descriptorHeap, 
            FilterType type,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputValuesResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputDepthsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsOctResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputVarianceResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputSmoothedVarianceResourceHandle,
            RWGpuResource* outputResourceHandle,
            float valueSigma,
            float depthSigma,
            float normalSigma,
            UINT numFilterPasses = 5,
            Mode filterMode = OutputFilteredValue,
            bool reverseFilterPassOrder = false,
            bool useCalculatedVariance = true);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObjects[FilterType::Count];
        RWGpuResource			            m_intermediateValueOutput;
        RWGpuResource			            m_intermediateVarianceOutputs[2];
        RWGpuResource			            m_filterWeightOutput;
        ConstantBuffer<AtrousWaveletTransformFilterConstantBuffer> m_CB;
        ConstantBuffer<AtrousWaveletTransformFilterConstantBuffer> m_CBfilterWeigth;
    };


    class CalculateVariance
    {
    public:
        enum FilterType {
            Bilateral5x5 = 0,
            Bilateral7x7,
            Count
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            ID3D12DescriptorHeap* descriptorHeap,
            FilterType type,
            UINT width,
            UINT height,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputValuesResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputDepthsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsOctResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle,
            float depthSigma,
            float normalSigma,
            bool useApproximateVariance = true);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObjects[FilterType::Count];
        ConstantBuffer<AtrousWaveletTransformFilterConstantBuffer> m_CB;
    };

    // ToDo bundle  RTAO ones together?
    class CalculateVariance2
    {
    public:
        enum FilterType {
            Bilateral5x5 = 0,
            Bilateral7x7,
            Count
        };

        void Release()
        {
            assert(0 && L"ToDo");
        }

        void Initialize(ID3D12Device* device);
        void Execute(
            ID3D12GraphicsCommandList* commandList,
            ID3D12DescriptorHeap* descriptorHeap,
            FilterType type,
            UINT width,
            UINT height,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputValuesResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputDepthsResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& inputNormalsOctResourceHandle,
            const D3D12_GPU_DESCRIPTOR_HANDLE& outputResourceHandle,
            float depthSigma,
            float normalSigma,
            bool useApproximateVariance = true);

    private:
        ComPtr<ID3D12RootSignature>         m_rootSignature;
        ComPtr<ID3D12PipelineState>         m_pipelineStateObjects[FilterType::Count];
        ConstantBuffer<AtrousWaveletTransformFilterConstantBuffer> m_CB;
    };
}