/**
 * Copyright (c) 2007, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.os;

/** {@hide} */
interface IKdcDmsiManager
{
	int readSerialno(int length, out byte[] buf);
	int readImei(int slotId, int length, out byte[] buf);
	int readMeid(int length, out byte[] buf);
	int readCalResult(int length, out byte[] buf);
	int writeCalResult(int length, in byte[] buf);
}

