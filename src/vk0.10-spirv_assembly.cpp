/*
 * Vulkan Samples Kit
 *
 * Copyright (C) 2015 Valve Corporation
 * Copyright (C) 2015 Google, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
VULKAN_SAMPLE_SHORT_DESCRIPTION
Demonsrate how to use SPIR-V shaders with inline assembly.
*/

#include <util_init.hpp>
#include <assert.h>
#include <string.h>
#include <cstdlib>
#include "cube_data.h"
#include "libspirv/libspirv.h"

// This sample is based on the template, but instead of using inline GLSL and calls to
// glslang to generate SPIR-V binaries, we use inline assembly and pass it to the
// SPIRV-Tools assembler.  This is one of many ways to generate SPIR-V binaries,
// which is the only shader representation accepted by Vulkan.

// The following inline SPIR-V assembly was generated by:
//   Populating template.vert and template.frag with contents of inlined GLSL from template sample
//   Running the following commands on Linux:
//     ./glslang/build/Standalone/bin/glslangValidator -V ./VulkanSamplesKit/template.vert -o ./VulkanSamplesKit/template.vert.spv
//     ./glslang/build/Standalone/bin/glslangValidator -V ./VulkanSamplesKit/template.frag -o ./VulkanSamplesKit/template.frag.spv
//     ./spirv-tools/build/spirv-dis ./VulkanSamplesKit/template.vert.spv | sed -e 's/\"/\\\"/g' -e 's/.*/\"&\\n\"/'
//     ./spirv-tools/build/spirv-dis ./VulkanSamplesKit/template.frag.spv | sed -e 's/\"/\\\"/g' -e 's/.*/\"&\\n\"/'

const std::string vertexSPIRV =
        "; SPIR-V\n"
        "; Version: 1.0\n"
        "; Generator: Khronos Glslang Reference Front End; 1\n"
        "; Bound: 49\n"
        "; Schema: 0\n"
        "               OpCapability Shader\n"
        "          %1 = OpExtInstImport \"GLSL.std.450\"\n"
        "               OpMemoryModel Logical GLSL450\n"
        "               OpEntryPoint Vertex %4 \"main\" %26 %11 %9 %47 %48\n"
        "               OpSource GLSL 140\n"
        "               OpSourceExtension \"GL_ARB_separate_shader_objects\"\n"
        "               OpSourceExtension \"GL_ARB_shading_language_420pack\"\n"
        "               OpName %4 \"main\"\n"
        "               OpName %9 \"texcoord\"\n"
        "               OpName %11 \"inTexCoords\"\n"
        "               OpName %15 \"gl_Position\"\n"
        "               OpName %17 \"buf\"\n"
        "               OpMemberName %17 0 \"mvp\"\n"
        "               OpName %19 \"ubuf\"\n"
        "               OpName %26 \"pos\"\n"
        "               OpName %47 \"gl_VertexIndex\"\n"
        "               OpName %48 \"gl_InstanceIndex\"\n"
        "               OpDecorate %9 Location 0\n"
        "               OpDecorate %11 Location 1\n"
        "               OpDecorate %15 BuiltIn Position\n"
        "               OpMemberDecorate %17 0 ColMajor\n"
        "               OpMemberDecorate %17 0 Offset 0\n"
        "               OpMemberDecorate %17 0 MatrixStride 16\n"
        "               OpDecorate %17 Block\n"
        "               OpDecorate %19 DescriptorSet 0\n"
        "               OpDecorate %19 Binding 0\n"
        "               OpDecorate %26 Location 0\n"
        "               OpDecorate %47 BuiltIn VertexIndex\n"
        "               OpDecorate %48 BuiltIn InstanceIndex\n"
        "          %2 = OpTypeVoid\n"
        "          %3 = OpTypeFunction %2\n"
        "          %6 = OpTypeFloat 32\n"
        "          %7 = OpTypeVector %6 2\n"
        "          %8 = OpTypePointer Output %7\n"
        "          %9 = OpVariable %8 Output\n"
        "         %10 = OpTypePointer Input %7\n"
        "         %11 = OpVariable %10 Input\n"
        "         %13 = OpTypeVector %6 4\n"
        "         %14 = OpTypePointer Output %13\n"
        "         %15 = OpVariable %14 Output\n"
        "         %16 = OpTypeMatrix %13 4\n"
        "         %17 = OpTypeStruct %16\n"
        "         %18 = OpTypePointer Uniform %17\n"
        "         %19 = OpVariable %18 Uniform\n"
        "         %20 = OpTypeInt 32 1\n"
        "         %21 = OpConstant %20 0\n"
        "         %22 = OpTypePointer Uniform %16\n"
        "         %25 = OpTypePointer Input %13\n"
        "         %26 = OpVariable %25 Input\n"
        "         %29 = OpTypeInt 32 0\n"
        "         %30 = OpConstant %29 1\n"
        "         %31 = OpTypePointer Output %6\n"
        "         %36 = OpConstant %29 2\n"
        "         %39 = OpConstant %29 3\n"
        "         %43 = OpConstant %6 2\n"
        "         %46 = OpTypePointer Input %20\n"
        "         %47 = OpVariable %46 Input\n"
        "         %48 = OpVariable %46 Input\n"
        "          %4 = OpFunction %2 None %3\n"
        "          %5 = OpLabel\n"
        "         %12 = OpLoad %7 %11\n"
        "               OpStore %9 %12\n"
        "         %23 = OpAccessChain %22 %19 %21\n"
        "         %24 = OpLoad %16 %23\n"
        "         %27 = OpLoad %13 %26\n"
        "         %28 = OpMatrixTimesVector %13 %24 %27\n"
        "               OpStore %15 %28\n"
        "         %32 = OpAccessChain %31 %15 %30\n"
        "         %33 = OpLoad %6 %32\n"
        "         %34 = OpFNegate %6 %33\n"
        "         %35 = OpAccessChain %31 %15 %30\n"
        "               OpStore %35 %34\n"
        "         %37 = OpAccessChain %31 %15 %36\n"
        "         %38 = OpLoad %6 %37\n"
        "         %40 = OpAccessChain %31 %15 %39\n"
        "         %41 = OpLoad %6 %40\n"
        "         %42 = OpFAdd %6 %38 %41\n"
        "         %44 = OpFDiv %6 %42 %43\n"
        "         %45 = OpAccessChain %31 %15 %36\n"
        "               OpStore %45 %44\n"
        "               OpReturn\n"
        "               OpFunctionEnd\n";


const std::string fragmentSPIRV =
        "; SPIR-V\n"
        "; Version: 1.0\n"
        "; Generator: Khronos Glslang Reference Front End; 1\n"
        "; Bound: 21\n"
        "; Schema: 0\n"
        "               OpCapability Shader\n"
        "          %1 = OpExtInstImport \"GLSL.std.450\"\n"
        "               OpMemoryModel Logical GLSL450\n"
        "               OpEntryPoint Fragment %4 \"main\" %17 %9\n"
        "               OpExecutionMode %4 OriginLowerLeft\n"
        "               OpSource GLSL 140\n"
        "               OpSourceExtension \"GL_ARB_separate_shader_objects\"\n"
        "               OpSourceExtension \"GL_ARB_shading_language_420pack\"\n"
        "               OpName %4 \"main\"\n"
        "               OpName %9 \"outColor\"\n"
        "               OpName %13 \"tex\"\n"
        "               OpName %17 \"texcoord\"\n"
        "               OpDecorate %9 Location 0\n"
        "               OpDecorate %13 DescriptorSet 0\n"
        "               OpDecorate %13 Binding 1\n"
        "               OpDecorate %17 Location 0\n"
        "          %2 = OpTypeVoid\n"
        "          %3 = OpTypeFunction %2\n"
        "          %6 = OpTypeFloat 32\n"
        "          %7 = OpTypeVector %6 4\n"
        "          %8 = OpTypePointer Output %7\n"
        "          %9 = OpVariable %8 Output\n"
        "         %10 = OpTypeImage %6 2D 0 0 0 1 Unknown\n"
        "         %11 = OpTypeSampledImage %10\n"
        "         %12 = OpTypePointer UniformConstant %11\n"
        "         %13 = OpVariable %12 UniformConstant\n"
        "         %15 = OpTypeVector %6 2\n"
        "         %16 = OpTypePointer Input %15\n"
        "         %17 = OpVariable %16 Input\n"
        "         %19 = OpConstant %6 0\n"
        "          %4 = OpFunction %2 None %3\n"
        "          %5 = OpLabel\n"
        "         %14 = OpLoad %11 %13\n"
        "         %18 = OpLoad %15 %17\n"
        "         %20 = OpImageSampleExplicitLod %7 %14 %18 Lod %19\n"
        "               OpStore %9 %20\n"
        "               OpReturn\n"
        "               OpFunctionEnd\n";

int main(int argc, char **argv)
{
    VkResult U_ASSERT_ONLY res;
    struct sample_info info = {};
    char sample_title[] = "SPIR-V Assembly";
    const bool depthPresent = true;

    init_global_layer_properties(info);
    init_instance_extension_names(info);
    init_device_extension_names(info);
    init_instance(info, sample_title);
    init_enumerate_device(info);
    init_device(info);
    info.width = info.height = 500;
    init_connection(info);
    init_window(info);
    init_swapchain_extension(info);
    init_command_pool(info);
    init_command_buffer(info);
    execute_begin_command_buffer(info);
    init_device_queue(info);
    init_swap_chain(info);
    init_depth_buffer(info);
    init_texture(info);
    init_uniform_buffer(info);
    init_descriptor_and_pipeline_layouts(info, true);
    init_renderpass(info, depthPresent);

    /* VULKAN_KEY_START */

    // Init the assembler context
    spv_context spvContext = spvContextCreate();

    // Convert the vertex assembly into binary format
    spv_binary vertexBinary = {};
    spv_diagnostic vertexDiag = {};
    spv_result_t vertexResult = spvTextToBinary(spvContext, vertexSPIRV.c_str(), vertexSPIRV.length(), &vertexBinary, &vertexDiag);
    if (vertexDiag) {
        printf("Diagnostic info from vertex shader:\n");
        spvDiagnosticPrint(vertexDiag);
    }
    assert(vertexResult == SPV_SUCCESS);

    // Convert the fragment assembly into binary format
    spv_binary fragmentBinary = {};
    spv_diagnostic fragmentDiag = {};
    spv_result_t fragmentResult = spvTextToBinary(spvContext, fragmentSPIRV.c_str(), fragmentSPIRV.length(), &fragmentBinary, &fragmentDiag);
    if (fragmentDiag) {
        printf("Diagnostic info from fragment shader:\n");
        spvDiagnosticPrint(fragmentDiag);
    }
    assert(fragmentResult == SPV_SUCCESS);

    info.shaderStages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.shaderStages[0].pNext  = NULL;
    info.shaderStages[0].pSpecializationInfo = NULL;
    info.shaderStages[0].flags = 0;
    info.shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    info.shaderStages[0].pName = "main";
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    // Use wordCount and code pointers from the spv_binary
    moduleCreateInfo.codeSize = vertexBinary->wordCount * sizeof(unsigned int);
    moduleCreateInfo.pCode = vertexBinary->code;
    res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &info.shaderStages[0].module);
    assert(res == VK_SUCCESS);

    info.shaderStages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.shaderStages[1].pNext  = NULL;
    info.shaderStages[1].pSpecializationInfo = NULL;
    info.shaderStages[1].flags = 0;
    info.shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    info.shaderStages[1].pName = "main";
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    // Use wordCount and code pointers from the spv_binary
    moduleCreateInfo.codeSize = fragmentBinary->wordCount * sizeof(unsigned int);
    moduleCreateInfo.pCode = fragmentBinary->code;
    res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &info.shaderStages[1].module);
    assert(res == VK_SUCCESS);

    // Clean up the diagnostics
    spvDiagnosticDestroy(vertexDiag);
    spvDiagnosticDestroy(fragmentDiag);

    // Clean up the assembler context
    spvContextDestroy(spvContext);

    /* VULKAN_KEY_END */

    init_framebuffers(info, depthPresent);
    init_vertex_buffer(info, g_vb_texture_Data, sizeof(g_vb_texture_Data),
                       sizeof(g_vb_texture_Data[0]), true);
    init_descriptor_pool(info, true);
    init_descriptor_set(info, true);
    init_pipeline_cache(info);
    init_pipeline(info, depthPresent);
    init_presentable_image(info);

    VkClearValue clear_values[2];
    init_clear_color_and_depth(info, clear_values);

    VkRenderPassBeginInfo rp_begin;
    init_render_pass_begin_info(info, rp_begin);
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;

    vkCmdBeginRenderPass(info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                  info.pipeline);
    vkCmdBindDescriptorSets(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline_layout,
            0, NUM_DESCRIPTOR_SETS, info.desc_set.data(), 0, NULL);

    const VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(info.cmd, 0, 1, &info.vertex_buffer.buf, offsets);

    init_viewports(info);
    init_scissors(info);

    vkCmdDraw(info.cmd, 12 * 3, 1, 0, 0);
    vkCmdEndRenderPass(info.cmd);

    execute_pre_present_barrier(info);

    res = vkEndCommandBuffer(info.cmd);
    assert(res == VK_SUCCESS);

    VkFence drawFence = {};
    init_fence(info, drawFence);
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkSubmitInfo submit_info = {};
    init_submit_info(info, submit_info, pipe_stage_flags);

    /* Queue the command buffer for execution */
    res = vkQueueSubmit(info.queue, 1, &submit_info, drawFence);
    assert(res == VK_SUCCESS);

    /* Now present the image in the window */
    VkPresentInfoKHR present = {};
    init_present_info(info, present);

    /* Make sure command buffer is finished before presenting */
    do {
        res = vkWaitForFences(info.device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
    } while(res == VK_TIMEOUT);
    assert(res == VK_SUCCESS);
    res = vkQueuePresentKHR(info.queue, &present);
    assert(res == VK_SUCCESS);

    wait_seconds(1);

    vkDestroyFence(info.device, drawFence, NULL);
    vkDestroySemaphore(info.device, info.presentCompleteSemaphore, NULL);
    destroy_pipeline(info);
    destroy_pipeline_cache(info);
    destroy_textures(info);
    destroy_descriptor_pool(info);
    destroy_vertex_buffer(info);
    destroy_framebuffers(info);
    destroy_shaders(info);
    destroy_renderpass(info);
    destroy_descriptor_and_pipeline_layouts(info);
    destroy_uniform_buffer(info);
    destroy_depth_buffer(info);
    destroy_swap_chain(info);
    destroy_command_buffer(info);
    destroy_command_pool(info);
    destroy_window(info);
    destroy_device(info);
    destroy_instance(info);
}
