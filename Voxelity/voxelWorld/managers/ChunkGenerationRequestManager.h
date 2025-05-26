//
// Created by adrian on 17/05/25.
//

#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <GLT.h>
#include <memory>

#include <optional>

struct VoxelStorage;
class ChunkGenerationThread;
class ChunkStorage;
class IChunkGenerator;

/**
 * @brief Manages requests for chunk generation and updating chunks around the player.
 *
 * This class handles scheduling chunk generation tasks in a separate thread,
 * updating chunks near the player’s position, applying generated chunk data,
 * and updating neighboring chunks as needed.
 */
class ChunkGenerationRequestManager {
public:

    /**
    * @brief Constructs the manager with a given chunk generator.
    *
    * @param generator Unique pointer to an IChunkGenerator instance responsible for actual chunk generation logic.
    */
    explicit ChunkGenerationRequestManager(std::unique_ptr<IChunkGenerator> generator);

    /**
    * @brief Destructor ensures proper cleanup of resources.
    */
    ~ChunkGenerationRequestManager();

    /**
     * @brief Updates chunks around the player's current chunk position.
     *
     * Requests generation for new chunks in range and manages chunk lifecycle.
     *
     * @param playerChunkPos Current chunk coordinate of the player.
     * @param worldChunkData Reference to the world chunk data storage.
     */
    void updateChunksAround(const glm::ivec3 & playerChunkPos, ChunkStorage &worldChunkData);

    /**
     * @brief Processes chunks that are ready after generation.
     *
     * Integrates generated chunk data into the world and prepares them for rendering.
     *
     * @param worldChunkData Reference to the world chunk data storage.
     */
    void processReadyChunks(ChunkStorage& worldChunkData) const;

    /**
     * @brief Applies generated chunk data to the world at a specified position.
     *
     * @param pos Position of the chunk in chunk coordinates.
     * @param data The chunk data to apply.
     * @param worldChunkData Reference to the world chunk data storage.
     */
    static void applyChunkData(const glm::ivec3& pos, const VoxelStorage& data, ChunkStorage& worldChunkData);

    /**
    * @brief Updates neighbor chunks of the chunk at the given position.
    *
    * Useful to trigger re-meshing or state updates after changes.
    *
    * @param pos Position of the chunk whose neighbors are to be updated.
    * @param worldChunkData Reference to the world chunk data storage.
    */
    static void updateNeighbors(const glm::ivec3& pos, ChunkStorage& worldChunkData);

private:
    std::unique_ptr<ChunkGenerationThread> generationThread; ///< Thread managing chunk generation tasks.

    std::optional<glm::ivec3> lastChunkPosition;            ///< Last recorded player chunk position for update optimization.

    /**
    * @brief Requests generation of a chunk at the specified position.
    *
    * Called internally to schedule chunk generation work.
    *
    * @param pos Position of the chunk to generate.
    * @param worldChunkData Reference to the world chunk data storage.
    */
    void generateChunkAt(const glm::ivec3 & pos, ChunkStorage &worldChunkData) const;

    /**
    * @brief Checks if a given chunk position is within the render distance from the center position.
    *
    * Helps cull chunk generation requests outside the visible area.
    *
    * @param center Center chunk coordinate (usually player chunk position).
    * @param pos Chunk coordinate to check.
    * @return true if within render distance, false otherwise.
    */
    static bool isWithinRenderDistance(const glm::ivec3& center, const glm::ivec3& pos);
};

#endif //CHUNKLOADER_H
