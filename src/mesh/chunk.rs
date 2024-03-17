use crate::world::chunk::{self, Chunk, CHUNK_SIZE};
use bevy::{
    log::{debug, info},
    math::{FloatExt, Vec3},
    render::{
        mesh::{Indices, Mesh, PrimitiveTopology},
        render_asset::RenderAssetUsages,
    },
    ui::debug,
};
use rand::Rng;

// Individual blocks mesh data, idk if this is the best way to do this
pub struct MeshInfo {
    pub positions: Vec<[f32; 3]>,
    pub colours: Vec<[f32; 4]>,
    pub indices: Vec<u32>, // which vertices to build which triangle
    pub normals: Vec<[f32; 3]>,
    pub uvs: Vec<[f32; 2]>,
}

pub enum BlockFace {
    POSX,
    POSY, // TOP
    POSZ,
    NEGX,
    NEGZ,
    NEGY, // BOTTOM
}

pub fn generate_block_faces(
    faces: Option<Vec<BlockFace>>,
    tile_size: f32,
    block_id: u8,
    start_pos: Vec3,
) -> MeshInfo {
    let mut positions: Vec<[f32; 3]> = Vec::new();
    let mut indices: Vec<u32> = Vec::new();
    let mut colours: Vec<[f32; 4]> = Vec::new();
    let mut index: u32 = 0;
    let mut normals: Vec<[f32; 3]> = Vec::new();
    let mut uvs: Vec<[f32; 2]> = Vec::new();
    match faces {
        Some(f) => {
            for face in f {
                let x_min = ((block_id as f32 - tile_size) * 16.0) / 128.0;
                let x_max = (block_id as f32 * 16.0) / 128.0;
                let y_max = ((block_id / 8) as f32 * 16.0) / 128.0;
                //let y_max = (16.0 + ((block_id as f32 / 8.0) * 16.0)) / 128.0;
                let mut offset = 16.0;
                if block_id > 8 {
                    offset = 16.0 + (16.0 * (block_id / 8) as f32);
                }
                let y_min = (((block_id / 8) as f32 * 16.0) + offset) / 128.0;
                match face {
                    BlockFace::POSX => {
                        positions.extend(vec![
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z,
                            ],
                            [start_pos.x + tile_size, start_pos.y, start_pos.z],
                            [
                                start_pos.x + tile_size,
                                start_pos.y,
                                start_pos.z + tile_size,
                            ],
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                        ]);
                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);
                        normals.extend(vec![
                            [1.0, 0.0, 0.0],
                            [1.0, 0.0, 0.0],
                            [1.0, 0.0, 0.0],
                            [1.0, 0.0, 0.0],
                        ]);

                        indices.extend(vec![
                            index,
                            index + 3,
                            index + 2,
                            index + 0,
                            index + 2,
                            index + 1,
                        ]);
                    }
                    BlockFace::POSY => {
                        positions.extend(vec![
                            [
                                start_pos.x,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                            [start_pos.x, start_pos.y + tile_size, start_pos.z],
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z,
                            ],
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                        ]);
                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);
                        normals.extend(vec![
                            [0.0, 1.0, 0.0],
                            [0.0, 1.0, 0.0],
                            [0.0, 1.0, 0.0],
                            [0.0, 1.0, 0.0],
                        ]);

                        indices.extend(vec![
                            index,
                            index + 3,
                            index + 2,
                            index + 0,
                            index + 2,
                            index + 1,
                        ]);
                    }
                    BlockFace::POSZ => {
                        positions.extend(vec![
                            [
                                start_pos.x,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                            [start_pos.x, start_pos.y, start_pos.z + tile_size],
                            [
                                start_pos.x + tile_size,
                                start_pos.y,
                                start_pos.z + tile_size,
                            ],
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                        ]);
                        normals.extend(vec![
                            [0.0, 0.0, 1.0],
                            [0.0, 0.0, 1.0],
                            [0.0, 0.0, 1.0],
                            [0.0, 0.0, 1.0],
                        ]);
                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);

                        indices.extend(vec![
                            index,
                            index + 1,
                            index + 2,
                            index + 3,
                            index + 0,
                            index + 2,
                        ]);
                    }
                    BlockFace::NEGX => {
                        positions.extend(vec![
                            [start_pos.x, start_pos.y + tile_size, start_pos.z],
                            [start_pos.x, start_pos.y, start_pos.z],
                            [start_pos.x, start_pos.y, start_pos.z + tile_size],
                            [
                                start_pos.x,
                                start_pos.y + tile_size,
                                start_pos.z + tile_size,
                            ],
                        ]);
                        normals.extend(vec![
                            [-1.0, 0.0, 0.0],
                            [-1.0, 0.0, 0.0],
                            [-1.0, 0.0, 0.0],
                            [-1.0, 0.0, 0.0],
                        ]);

                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);
                        indices.extend(vec![
                            index,
                            index + 1,
                            index + 2,
                            index + 3,
                            index + 0,
                            index + 2,
                        ]);
                    }
                    BlockFace::NEGY => {
                        positions.extend(vec![
                            [start_pos.x, start_pos.y, start_pos.z + tile_size],
                            [start_pos.x, start_pos.y, start_pos.z],
                            [start_pos.x + tile_size, start_pos.y, start_pos.z],
                            [
                                start_pos.x + tile_size,
                                start_pos.y,
                                start_pos.z + tile_size,
                            ],
                        ]);
                        normals.extend(vec![
                            [0.0, -1.0, 0.0],
                            [0.0, -1.0, 0.0],
                            [0.0, -1.0, 0.0],
                            [0.0, -1.0, 0.0],
                        ]);
                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);
                        indices.extend(vec![
                            index,
                            index + 1,
                            index + 2,
                            index + 3,
                            index + 0,
                            index + 2,
                        ]);
                    }
                    BlockFace::NEGZ => {
                        positions.extend(vec![
                            [start_pos.x, start_pos.y + tile_size, start_pos.z],
                            [start_pos.x, start_pos.y, start_pos.z],
                            [start_pos.x + tile_size, start_pos.y, start_pos.z],
                            [
                                start_pos.x + tile_size,
                                start_pos.y + tile_size,
                                start_pos.z,
                            ],
                        ]);
                        normals.extend(vec![
                            [0.0, 0.0, -1.0],
                            [0.0, 0.0, -1.0],
                            [0.0, 0.0, -1.0],
                            [0.0, 0.0, -1.0],
                        ]);
                        uvs.extend(vec![
                            [x_min, y_max],
                            [x_min, y_min],
                            [x_max, y_min],
                            [x_max, y_max],
                        ]);

                        indices.extend(vec![
                            index,
                            index + 3,
                            index + 2,
                            index + 0,
                            index + 2,
                            index + 1,
                        ]);
                    }
                }
                /*colours.extend(vec![
                    [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
                    [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
                    [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
                    [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
                ]);*/

                /*colours.extend(vec![
                    [
                        start_pos.x / 16.0,
                        start_pos.y / 16.0,
                        start_pos.z / 16.0,
                        1.0,
                    ],
                    [
                        start_pos.x / 16.0,
                        start_pos.y / 16.0,
                        start_pos.z / 16.0,
                        1.0,
                    ],
                    [
                        start_pos.x / 16.0,
                        start_pos.y / 16.0,
                        start_pos.z / 16.0,
                        1.0,
                    ],
                    [
                        start_pos.x / 16.0,
                        start_pos.y / 16.0,
                        start_pos.z / 16.0,
                        1.0,
                    ],
                ]);*/
                colours.extend(match block_id {
                    0 => [[0.1, 0.75, 0.1, 1.0]; 4],
                    1 => [[0.7, 0.01, 0.62, 1.0]; 4],
                    2 => [[0.3, 0.3, 0.75, 1.0]; 4],
                    3 => [[1.0, 0.4, 0.4, 1.0]; 4],
                    4 => [[0.0, 0.0, 0.0, 1.0]; 4],
                    5 => [[0.6, 0.6, 0.1, 1.0]; 4],
                    6 => [[0.7, 0.7, 1.0, 1.0]; 4],
                    7 => [[0.8, 0.1, 0.8, 1.0]; 4],
                    _ => [[0.3, 0.8, 0.8, 1.0]; 4],
                });

                /*uvs.extend(vec![
                    [x_min, y_min],
                    [x_min, y_max],
                    [x_max, y_max],
                    [x_min, y_min],
                ]);*/
                /*uvs.extend(vec![
                    [x_min, y_max],
                    [x_min, y_min],
                    [x_max, x_min],
                    [x_max, y_max],
                ]);*/
                index += 4;
            }
        }
        None => (),
    }

    MeshInfo {
        positions,
        colours,
        normals,
        indices,
        uvs,
    }
}
